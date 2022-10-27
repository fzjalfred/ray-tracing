// Termm-Fall 2022

#include "A3.hpp"
#include "scene_lua.hpp"
using namespace std;

#include "cs488-framework/GlErrorCheck.hpp"
#include "cs488-framework/MathUtils.hpp"
#include "GeometryNode.hpp"
#include "JointNode.hpp"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

static bool show_gui = true;

const size_t CIRCLE_PTS = 48;

float translationRatio = 76.0f;

int xforms;

double prevX;
double prevY;

//----------------------------------------------------------------------------------------
// Constructor
A3::A3(const std::string & luaSceneFile)
	: m_luaSceneFile(luaSceneFile),
	  m_positionAttribLocation(0),
	  m_normalAttribLocation(0),
	  m_vao_meshData(0),
	  m_vbo_vertexPositions(0),
	  m_vbo_vertexNormals(0),
	  m_vao_arcCircle(0),
	  m_vbo_arcCircle(0)
{

}

//----------------------------------------------------------------------------------------
// Destructor
A3::~A3()
{

}

void A3::resetAll() {
	resetPos();
	resetJoints();
	resetOrientation();
}

void A3::resetPos() {
	m_translation = mat4(1.0f);
}

void A3::resetOrientation() {
	m_rotation = mat4(1.0f);
}

void A3::resetJoints() {
	for (auto i: nodesTable) {
		if (i.second->m_nodeType == NodeType::JointNode) {
			i.second->jointRotate = mat4();
			((JointNode*)i.second)->reset();
		}
	}
	headNode->reset();
	undos = std::stack<std::vector<JointRecord>>();
	redos = std::stack<std::vector<JointRecord>>();
	isRedoUndoed = false;
	isJointChange = false;
}

void A3::undo() {
	if (undos.empty()) return;
	std::vector<JointRecord> acts = undos.top();
	undos.pop();
	redos.push(acts);

	for (auto act: acts) {
		act.printJoint();
		act.print();
		act.undo();
		act.printJoint();
	}
	
}

void A3::redo() {
	if (redos.empty()) return;
	std::vector<JointRecord> acts = redos.top();
	redos.pop();
	undos.push(acts);
	
	for (auto act: acts) {
		act.execute();
	}
	
}

void A3::traverse(SceneNode& root, int& count, std::vector<bool>& selected) {
		count += 1;
		selected.push_back(false);
		nodesTable.emplace(root.m_nodeId, &root);
		if (root.m_name == "neckJoint") {
			headNode = (JointNode*)&root;
		}
		for (auto i: root.children) {
			traverse(*i, count, selected);
			i->parent = &root;
		}
}

//----------------------------------------------------------------------------------------
/*
 * Called once, at program start.
 */
void A3::init()
{
	// Set the background colour.
	glClearColor(0.2, 0.5, 0.3, 1.0);

	createShaderProgram();

	glGenVertexArrays(1, &m_vao_arcCircle);
	glGenVertexArrays(1, &m_vao_meshData);
	enableVertexShaderInputSlots();

	processLuaSceneFile(m_luaSceneFile);

	// Load and decode all .obj files at once here.  You may add additional .obj files to
	// this list in order to support rendering additional mesh types.  All vertex
	// positions, and normals will be extracted and stored within the MeshConsolidator
	// class.
	unique_ptr<MeshConsolidator> meshConsolidator (new MeshConsolidator{
			getAssetFilePath("cube.obj"),
			getAssetFilePath("sphere.obj"),
			getAssetFilePath("suzanne.obj"),
			getAssetFilePath("head.obj"),
			getAssetFilePath("head_decorate.obj"),
			getAssetFilePath("head_black_body.obj"),
			getAssetFilePath("torso_black_body.obj"),
			getAssetFilePath("torso.obj"),
			getAssetFilePath("torso_decorate.obj"),
			getAssetFilePath("face.obj"),
			getAssetFilePath("eyes.obj"),
			getAssetFilePath("nose.obj"),
//arm
			getAssetFilePath("hand.obj"),
			getAssetFilePath("ArmUp.obj"),
			getAssetFilePath("ArmLower.obj"),
//leg
			getAssetFilePath("LegUp.obj"),
			getAssetFilePath("LegLower.obj"),
			getAssetFilePath("foot.obj")

	});

	int count = 0;
	traverse(*m_rootNode, count, selected);
	std::cout<<"Nodes in total: "<<count<<std::endl;

	// Acquire the BatchInfoMap from the MeshConsolidator.
	meshConsolidator->getBatchInfoMap(m_batchInfoMap);

	// Take all vertex data within the MeshConsolidator and upload it to VBOs on the GPU.
	uploadVertexDataToVbos(*meshConsolidator);

	mapVboDataToVertexShaderInputLocations();

	initPerspectiveMatrix();

	initViewMatrix();

	initLightSources();



	// Exiting the current scope calls delete automatically on meshConsolidator freeing
	// all vertex data resources.  This is fine since we already copied this data to
	// VBOs on the GPU.  We have no use for storing vertex data on the CPU side beyond
	// this point.
}

//----------------------------------------------------------------------------------------
void A3::processLuaSceneFile(const std::string & filename) {
	// This version of the code treats the Lua file as an Asset,
	// so that you'd launch the program with just the filename
	// of a puppet in the Assets/ directory.
	// std::string assetFilePath = getAssetFilePath(filename.c_str());
	// m_rootNode = std::shared_ptr<SceneNode>(import_lua(assetFilePath));

	// This version of the code treats the main program argument
	// as a straightforward pathname.
	m_rootNode = std::shared_ptr<SceneNode>(import_lua(filename));
	if (!m_rootNode) {
		std::cerr << "Could Not Open " << filename << std::endl;
	}
}

//----------------------------------------------------------------------------------------
void A3::createShaderProgram()
{
	m_shader.generateProgramObject();
	m_shader.attachVertexShader( getAssetFilePath("VertexShader.vs").c_str() );
	m_shader.attachFragmentShader( getAssetFilePath("FragmentShader.fs").c_str() );
	m_shader.link();

	m_shader_arcCircle.generateProgramObject();
	m_shader_arcCircle.attachVertexShader( getAssetFilePath("arc_VertexShader.vs").c_str() );
	m_shader_arcCircle.attachFragmentShader( getAssetFilePath("arc_FragmentShader.fs").c_str() );
	m_shader_arcCircle.link();
}

//----------------------------------------------------------------------------------------
void A3::enableVertexShaderInputSlots()
{
	//-- Enable input slots for m_vao_meshData:
	{
		glBindVertexArray(m_vao_meshData);

		// Enable the vertex shader attribute location for "position" when rendering.
		m_positionAttribLocation = m_shader.getAttribLocation("position");
		glEnableVertexAttribArray(m_positionAttribLocation);

		// Enable the vertex shader attribute location for "normal" when rendering.
		m_normalAttribLocation = m_shader.getAttribLocation("normal");
		glEnableVertexAttribArray(m_normalAttribLocation);

		CHECK_GL_ERRORS;
	}


	//-- Enable input slots for m_vao_arcCircle:
	{
		glBindVertexArray(m_vao_arcCircle);

		// Enable the vertex shader attribute location for "position" when rendering.
		m_arc_positionAttribLocation = m_shader_arcCircle.getAttribLocation("position");
		glEnableVertexAttribArray(m_arc_positionAttribLocation);

		CHECK_GL_ERRORS;
	}

	// Restore defaults
	glBindVertexArray(0);
}

//----------------------------------------------------------------------------------------
void A3::uploadVertexDataToVbos (
		const MeshConsolidator & meshConsolidator
) {
	// Generate VBO to store all vertex position data
	{
		glGenBuffers(1, &m_vbo_vertexPositions);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexPositions);

		glBufferData(GL_ARRAY_BUFFER, meshConsolidator.getNumVertexPositionBytes(),
				meshConsolidator.getVertexPositionDataPtr(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		CHECK_GL_ERRORS;
	}

	// Generate VBO to store all vertex normal data
	{
		glGenBuffers(1, &m_vbo_vertexNormals);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexNormals);

		glBufferData(GL_ARRAY_BUFFER, meshConsolidator.getNumVertexNormalBytes(),
				meshConsolidator.getVertexNormalDataPtr(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		CHECK_GL_ERRORS;
	}

	// Generate VBO to store the trackball circle.
	{
		glGenBuffers( 1, &m_vbo_arcCircle );
		glBindBuffer( GL_ARRAY_BUFFER, m_vbo_arcCircle );

		float *pts = new float[ 2 * CIRCLE_PTS ];
		for( size_t idx = 0; idx < CIRCLE_PTS; ++idx ) {
			float ang = 2.0 * M_PI * float(idx) / CIRCLE_PTS;
			pts[2*idx] = cos( ang );
			pts[2*idx+1] = sin( ang );
		}

		glBufferData(GL_ARRAY_BUFFER, 2*CIRCLE_PTS*sizeof(float), pts, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		CHECK_GL_ERRORS;
	}
}

//----------------------------------------------------------------------------------------
void A3::mapVboDataToVertexShaderInputLocations()
{
	// Bind VAO in order to record the data mapping.
	glBindVertexArray(m_vao_meshData);

	// Tell GL how to map data from the vertex buffer "m_vbo_vertexPositions" into the
	// "position" vertex attribute location for any bound vertex shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexPositions);
	glVertexAttribPointer(m_positionAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Tell GL how to map data from the vertex buffer "m_vbo_vertexNormals" into the
	// "normal" vertex attribute location for any bound vertex shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexNormals);
	glVertexAttribPointer(m_normalAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	//-- Unbind target, and restore default values:
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	CHECK_GL_ERRORS;

	// Bind VAO in order to record the data mapping.
	glBindVertexArray(m_vao_arcCircle);

	// Tell GL how to map data from the vertex buffer "m_vbo_arcCircle" into the
	// "position" vertex attribute location for any bound vertex shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_arcCircle);
	glVertexAttribPointer(m_arc_positionAttribLocation, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	//-- Unbind target, and restore default values:
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
void A3::initPerspectiveMatrix()
{
	float aspect = ((float)m_windowWidth) / m_windowHeight;
	m_perpsective = glm::perspective(degreesToRadians(60.0f), aspect, 0.1f, 100.0f);
}


//----------------------------------------------------------------------------------------
void A3::initViewMatrix() {
	m_view = glm::lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f),
			vec3(0.0f, 1.0f, 0.0f));
}

//----------------------------------------------------------------------------------------
void A3::initLightSources() {
	// World-space position
	m_light.position = vec3(-10.0f, 15.0f, 10.0f);
	m_light.rgbIntensity = vec3(0.8f); // light
}

//----------------------------------------------------------------------------------------
void A3::uploadCommonSceneUniforms() {
	m_shader.enable();
	{
		//-- Set Perpsective matrix uniform for the scene:
		GLint location = m_shader.getUniformLocation("Perspective");
		glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(m_perpsective));
		CHECK_GL_ERRORS;

		location = m_shader.getUniformLocation("picking");
		glUniform1i( location, do_picking ? 1 : 0 );

		if (!do_picking) {
			//-- Set LightSource uniform for the scene:
			{
				location = m_shader.getUniformLocation("light.position");
				glUniform3fv(location, 1, value_ptr(m_light.position));
				location = m_shader.getUniformLocation("light.rgbIntensity");
				glUniform3fv(location, 1, value_ptr(m_light.rgbIntensity));
				CHECK_GL_ERRORS;
			}

			//-- Set background light ambient intensity
			{
				location = m_shader.getUniformLocation("ambientIntensity");
				vec3 ambientIntensity(0.25f);
				glUniform3fv(location, 1, value_ptr(ambientIntensity));
				CHECK_GL_ERRORS;
			}
		}
		
	}
	m_shader.disable();
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, before guiLogic().
 */
void A3::appLogic()
{
	// Place per frame, application logic here ...

	uploadCommonSceneUniforms();
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after appLogic(), but before the draw() method.
 */
void A3::guiLogic()
{
	if( !show_gui ) {
		return;
	}

	static bool firstRun(true);
	if (firstRun) {
		ImGui::SetNextWindowPos(ImVec2(50, 50));
		firstRun = false;
	}

	static bool showDebugWindow(true);
	ImGuiWindowFlags windowFlags(ImGuiWindowFlags_AlwaysAutoResize);
	float opacity(0.5f);

	ImGui::Begin("Properties", &showDebugWindow, ImVec2(100,100), opacity,
			windowFlags);


		// Add more gui elements here here ...
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("Application")) {
				if (ImGui::MenuItem("Reset Position (I)")) {
					resetPos();
				}
				if (ImGui::MenuItem("Reset Orientation (O)")) {
					resetOrientation();
				}
				if (ImGui::MenuItem("Reset Joint (S)")) {
					resetJoints();
				}
				if (ImGui::MenuItem("Reset All (A)")) {
					resetAll();
				}
				if( ImGui::MenuItem( "Quit (Q)" ) ) {
					glfwSetWindowShouldClose(m_window, GL_TRUE);
				}
				 ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit")) {
				if (ImGui::MenuItem("Undo (U)")) {
					// undo
					// undoOperation();
				}

				if (ImGui::MenuItem("Redo (R)")) {
					// redo
					// redoOperation();
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Options")) {
				ImGui::Checkbox("Circle (C)", &trackBallCircle);
				ImGui::Checkbox("Z-buffer (Z)", &zBuffer);
				ImGui::Checkbox("Backface culling (B)", &backfaceCulling);
				ImGui::Checkbox("Frontface culling (F)", &frontfaceCulling);
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		if( ImGui::RadioButton( "Position/Orientation (P)", &mode, 0) ) {

		}
		if( ImGui::RadioButton( "Joints (J)", &mode, 1) ) {

		}

		// Create Button, and check if it was clicked:
		if( ImGui::Button( "Quit Application" ) ) {
			glfwSetWindowShouldClose(m_window, GL_TRUE);
		}

		ImGui::Text( "Framerate: %.1f FPS", ImGui::GetIO().Framerate );

	ImGui::End();
}

//----------------------------------------------------------------------------------------
// Update mesh specific shader uniforms:
static void updateShaderUniforms(
		const ShaderProgram & shader,
		const GeometryNode & node,
		const glm::mat4 & viewMatrix,
		const glm::mat4 & modelMatrix,
		const std::vector<bool>& selected,
		bool do_picking = false
) {

	shader.enable();
	{
		//-- Set ModelView matrix:
		GLint location = shader.getUniformLocation("ModelView");
		mat4 modelView = viewMatrix * modelMatrix * node.trans;
		glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(modelView));
		CHECK_GL_ERRORS;

		//-- Set NormMatrix:
		location = shader.getUniformLocation("NormalMatrix");
		mat3 normalMatrix = glm::transpose(glm::inverse(mat3(modelView)));
		glUniformMatrix3fv(location, 1, GL_FALSE, value_ptr(normalMatrix));
		CHECK_GL_ERRORS;


		//-- Set Material values:
		location = shader.getUniformLocation("material.kd");
		vec3 kd = node.material.kd;
		// do_picking = random()%2;
		if (do_picking) {
			float r = float(node.m_nodeId&0xff) / 255.0f;
			float g = float((node.m_nodeId>>8)&0xff) / 255.0f;
			float b = float((node.m_nodeId>>16)&0xff) / 255.0f;
			glUniform3f( location, r, g, b );
		} else {
			if (selected[node.m_nodeId]) {
				kd = glm::vec3( 1.0, 1.0, 0.0 );
			}
			glUniform3fv(location, 1, value_ptr(kd));
		}
		
		CHECK_GL_ERRORS;
	}
	shader.disable();

}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after guiLogic().
 */
void A3::draw() {
	if (zBuffer) {
		glEnable( GL_DEPTH_TEST );
	}
	if (frontfaceCulling && backfaceCulling) {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT_AND_BACK);
	} else if (frontfaceCulling) {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
	} else if (backfaceCulling) {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

	renderSceneGraph(*m_rootNode);

	if (trackBallCircle) {
		glDisable( GL_DEPTH_TEST );
		renderArcCircle();
	}
	
}

//----------------------------------------------------------------------------------------
void A3::renderSceneGraph(const SceneNode & root) {

	// Bind the VAO once here, and reuse for all GeometryNode rendering below.
	glBindVertexArray(m_vao_meshData);

	// This is emphatically *not* how you should be drawing the scene graph in
	// your final implementation.  This is a non-hierarchical demonstration
	// in which we assume that there is a list of GeometryNodes living directly
	// underneath the root node, and that we can draw them in a loop.  It's
	// just enough to demonstrate how to get geometry and materials out of
	// a GeometryNode and onto the screen.

	// You'll want to turn this into recursive code that walks over the tree.
	// You can do that by putting a method in SceneNode, overridden in its
	// subclasses, that renders the subtree rooted at every node.  Or you
	// could put a set of mutually recursive functions in this class, which
	// walk down the tree from nodes of different types.

	// for (const SceneNode * node : root.children) {

	// 	if (node->m_nodeType != NodeType::GeometryNode)
	// 		continue;

	// 	const GeometryNode * geometryNode = static_cast<const GeometryNode *>(node);

	// 	updateShaderUniforms(m_shader, *geometryNode, m_view);


	// 	// Get the BatchInfo corresponding to the GeometryNode's unique MeshId.
	// 	BatchInfo batchInfo = m_batchInfoMap[geometryNode->meshId];

	// 	//-- Now render the mesh:
	// 	m_shader.enable();
	// 	glDrawArrays(GL_TRIANGLES, batchInfo.startIndex, batchInfo.numIndices);
	// 	m_shader.disable();
	// }
	mat4 puppetPos = root.get_transform();
	renderTreeNode(&root, m_translation*puppetPos*m_rotation*root.jointRotate*inverse(puppetPos));

	glBindVertexArray(0);
	CHECK_GL_ERRORS;
}

void A3::renderTreeNode(const SceneNode * root, mat4 model) {
	if (root->m_nodeType == NodeType::GeometryNode) {
		const GeometryNode * geometryNode = static_cast<const GeometryNode *>(root);
		updateShaderUniforms(m_shader, *geometryNode, m_view, model, selected, do_picking);

		// Get the BatchInfo corresponding to the GeometryNode's unique MeshId.
		BatchInfo batchInfo = m_batchInfoMap[geometryNode->meshId];

		//-- Now render the mesh:
		m_shader.enable();
		glDrawArrays(GL_TRIANGLES, batchInfo.startIndex, batchInfo.numIndices);
		m_shader.disable();
	}
	for (const SceneNode * node : root->children) {
		// std::cout<<*node<<std::endl;
		renderTreeNode(node, model*root->jointRotate*root->get_transform());
	}
}

//----------------------------------------------------------------------------------------
// Draw the trackball circle.
void A3::renderArcCircle() {
	glBindVertexArray(m_vao_arcCircle);

	m_shader_arcCircle.enable();
		GLint m_location = m_shader_arcCircle.getUniformLocation( "M" );
		float aspect = float(m_framebufferWidth)/float(m_framebufferHeight);
		glm::mat4 M;
		if( aspect > 1.0 ) {
			M = glm::scale( glm::mat4(), glm::vec3( 0.5/aspect, 0.5, 1.0 ) );
		} else {
			M = glm::scale( glm::mat4(), glm::vec3( 0.5, 0.5*aspect, 1.0 ) );
		}
		glUniformMatrix4fv( m_location, 1, GL_FALSE, value_ptr( M ) );
		glDrawArrays( GL_LINE_LOOP, 0, CIRCLE_PTS );
	m_shader_arcCircle.disable();

	glBindVertexArray(0);
	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
/*
 * Called once, after program is signaled to terminate.
 */
void A3::cleanup()
{

}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles cursor entering the window area events.
 */
bool A3::cursorEnterWindowEvent (
		int entered
) {
	bool eventHandled(false);

	// Fill in with event handling code...

	return eventHandled;
}

void A3::performPosition(double xPos, double yPos) {
	// double xNPos = xPos/m_framebufferWidth;
	// double yNPos = yPos/m_framebufferHeight;
	// double prevXN = prevX /m_framebufferWidth;
	// double prevYN = prevY /m_framebufferHeight;
	float centerX = m_framebufferWidth/2.0f;
	float centerY = m_framebufferHeight/2.0f;
	float trackballRadius = std::min(m_framebufferWidth / 4.0f, m_framebufferHeight / 4.0f);
	vec3 center2mouse = vec3(xPos - centerX, -yPos + centerY, 0);
	center2mouse.z = sqrt(trackballRadius*trackballRadius - ((xPos-centerX)*(xPos-centerX) + (yPos-centerY)*(yPos-centerY)));
	if (std::isnan(center2mouse.z)) {
		center2mouse.z = 0;
	}
	// std::cout<<"x: "<<center2mouse.x<<std::endl;
	// std::cout<<"y: "<<center2mouse.y<<std::endl;
	// std::cout<<"z: "<<center2mouse.z<<std::endl;

	if (leftMousePressed) {
		mat4 translate = mat4(1.0f);
		translate[3][0] = (center2mouse.x - prevTrackballPoint.x)/translationRatio;
		translate[3][1] = (center2mouse.y - prevTrackballPoint.y)/translationRatio;
		m_translation *= translate;
	}

	if (middleMousePressed) {
		mat4 translate = mat4(1.0f);
		translate[3][2] = -(center2mouse.y - prevTrackballPoint.y)/translationRatio;
		m_translation *= translate;
	}

	if (rightMousePressed) {
		vec3 rotation_axis = cross(center2mouse, prevTrackballPoint);
		if ( ((xPos-centerX)*(xPos-centerX) + (yPos-centerY)*(yPos-centerY)) <= trackballRadius*trackballRadius && rotation_axis != vec3(0.0f)) {
			// std::cout<<"inside"<<std::endl;
			// std::cout<<"x: "<<rotation_axis.x<<std::endl;
			// std::cout<<"y: "<<rotation_axis.y<<std::endl;
			// std::cout<<"z: "<<rotation_axis.z<<std::endl;
			m_rotation = glm::rotate(mat4(), -0.01f, rotation_axis)*m_rotation;
		} else {
			// std::cout<<"outside"<<std::endl;
			m_rotation = glm::rotate(mat4(), (float)(-yPos + prevY)*0.01f, vec3(0.0f,0.0f,1.0f)) *m_rotation;
		}
	}

	prevTrackballPoint = center2mouse;
	
}

void A3::performRotate(double xPos, double yPos) {
	if (middleMousePressed) {
		double factor = yPos - prevY;
		for (int i = 0; i<selected.size(); i++) {
			if (selected[i] == true && nodesTable[i]->m_nodeType == NodeType::JointNode) {
				JointNode* jn = (JointNode*)nodesTable[i]; 
				float angle_factor = factor/100;
				jn->rotate(angle_factor);
				isJointChange = true;
			}
		}
		if (isJointChange && isRedoUndoed) {
			undos = std::stack<std::vector<JointRecord>>();
			redos = std::stack<std::vector<JointRecord>>();
			isRedoUndoed = false;
		}
	}
	if (rightMousePressed) {
		double factor = (xPos - prevX)/100;
		// float headAngle = headNode->headAngle;
		// //std::cout<<headAngle<<std::endl;
		// && (headAngle + factor)<60 && (headAngle + factor)>-60
		if(selected[headNode->m_nodeId] ) {
			headNode->rotate('y', factor);
			isJointChange = true;
		}
		if (isJointChange && isRedoUndoed) {
			undos = std::stack<std::vector<JointRecord>>();
			redos = std::stack<std::vector<JointRecord>>();
			isRedoUndoed = false;
		}
	}
	
}


//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse cursor movement events.
 */
bool A3::mouseMoveEvent (
		double xPos,
		double yPos
) {
	bool eventHandled(false);

	// Fill in with event handling code...
	if (!ImGui::IsMouseHoveringAnyWindow()) {
		// Fill in with event handling code...
		switch (mode) {
			case 0: // Position and Rotation
				performPosition(xPos, yPos);
				eventHandled = true;
				break;
			case 1:
				performRotate(xPos, yPos);
				eventHandled = true;
				break;
			default:
				break; 
		}


		prevX = xPos;
		prevY = yPos;
	}
	// std::cout<<"x: "<<xPos<<std::endl;
    // std::cout<<"y: "<<yPos<<std::endl;

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse button events.
 */
bool A3::mouseButtonInputEvent (
		int button,
		int actions,
		int mods
) {
	bool eventHandled(false);

	// Fill in with event handling code...
	if (button == GLFW_MOUSE_BUTTON_1 && actions == GLFW_PRESS) {
			leftMousePressed = true;
			if (mode == 1) {
				double xpos, ypos;
				glfwGetCursorPos( m_window, &xpos, &ypos );

				do_picking = true;

				uploadCommonSceneUniforms();
				glClearColor(1.0, 1.0, 1.0, 1.0 );
				glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
				glClearColor(0.35, 0.35, 0.35, 1.0);

				draw();

				// I don't know if these are really necessary anymore.
				// glFlush();
				// glFinish();

				CHECK_GL_ERRORS;

				// Ugly -- FB coordinates might be different than Window coordinates
				// (e.g., on a retina display).  Must compensate.
				xpos *= double(m_framebufferWidth) / double(m_windowWidth);
				// WTF, don't know why I have to measure y relative to the bottom of
				// the window in this case.
				ypos = m_windowHeight - ypos;
				ypos *= double(m_framebufferHeight) / double(m_windowHeight);

				GLubyte buffer[ 4 ] = { 0, 0, 0, 0 };
				// A bit ugly -- don't want to swap the just-drawn false colours
				// to the screen, so read from the back buffer.
				glReadBuffer( GL_BACK );
				// Actually read the pixel at the mouse location.
				glReadPixels( int(xpos), int(ypos), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, buffer );
				CHECK_GL_ERRORS;

				// Reassemble the object ID.
				unsigned int what = buffer[0] + (buffer[1] << 8) + (buffer[2] << 16);
				
				if (0<=what && what<selected.size() && nodesTable[what]->parent->m_nodeType == NodeType::JointNode) {
					selected[what] = !selected[what];
					selected[nodesTable[what]->parent->m_nodeId] = !selected[nodesTable[what]->parent->m_nodeId];
				}
				

				do_picking = false;

				CHECK_GL_ERRORS;
			}
		}
		if (button == GLFW_MOUSE_BUTTON_1 && actions == GLFW_RELEASE) {
			leftMousePressed = false;
		}
		if (button == GLFW_MOUSE_BUTTON_2 && actions == GLFW_PRESS) {
			recordCmdCreate();
			rightMousePressed = true;
		}
		if (button == GLFW_MOUSE_BUTTON_2 && actions == GLFW_RELEASE) {
			recordAllJoints();
			rightMousePressed = false;
		}
		if (button == GLFW_MOUSE_BUTTON_3 && actions == GLFW_PRESS) {
			recordCmdCreate();
			middleMousePressed = true;
		}
		if (button == GLFW_MOUSE_BUTTON_3 && actions == GLFW_RELEASE) {
			recordAllJoints();
			middleMousePressed = false;
		}
	return eventHandled;
}

void A3::recordCmdCreate() {
	cur_cmds = std::vector<JointRecord>();
	for (auto i: nodesTable) {
		if (i.second->m_nodeType == NodeType::JointNode) {
			cur_cmds.push_back(JointRecord((JointNode*)i.second));
		}
	}
}

void A3::recordAllJoints() {
	if (mode == 1 && isJointChange) {
		for (auto& act: cur_cmds) {
			act.record();
		}
		undos.push(cur_cmds);
	}
	
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse scroll wheel events.
 */
bool A3::mouseScrollEvent (
		double xOffSet,
		double yOffSet
) {
	bool eventHandled(false);

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles window resize events.
 */
bool A3::windowResizeEvent (
		int width,
		int height
) {
	bool eventHandled(false);
	initPerspectiveMatrix();
	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles key input events.
 */
bool A3::keyInputEvent (
		int key,
		int action,
		int mods
) {
	bool eventHandled(false);

	if( action == GLFW_PRESS ) {
		if( key == GLFW_KEY_M ) {
			show_gui = !show_gui;
			eventHandled = true;
		}
		// applications
		if( key == GLFW_KEY_Q ) {
			glfwSetWindowShouldClose(m_window, GL_TRUE);
			eventHandled = true;
		}
		if( key == GLFW_KEY_I ) {
			resetPos();
			eventHandled = true;
		}
		if( key == GLFW_KEY_O ) {
			resetOrientation();
			eventHandled = true;
		}
		if( key == GLFW_KEY_S ) {
			resetJoints();
			eventHandled = true;
		}
		if( key == GLFW_KEY_A ) {
			resetAll();
			eventHandled = true;
		}
		// options
		if( key == GLFW_KEY_C ) {
			trackBallCircle = !trackBallCircle;
			eventHandled = true;
		}
		if( key == GLFW_KEY_Z ) {
			zBuffer = !zBuffer;
			eventHandled = true;
		}
		if( key == GLFW_KEY_B ) {
			backfaceCulling = !backfaceCulling;
			eventHandled = true;
		}
		if( key == GLFW_KEY_F ) {
			frontfaceCulling = !frontfaceCulling;
			eventHandled = true;
		}
		// mode
		if ( key == GLFW_KEY_P ) {
			mode = 0;
			eventHandled = true;
		}
		if ( key == GLFW_KEY_J ) {
			mode = 1;
			eventHandled = true;
		}
		// undo redo
		if (key == GLFW_KEY_R) {
			redo();
			isRedoUndoed = true;
			eventHandled = true;
		}
		if (key == GLFW_KEY_U) {
			undo();
			isRedoUndoed = true;
			eventHandled = true;
		}
	}
	// Fill in with event handling code...
	// if (isRedoing) {
	// 	redo();
	// 	isRedoUndoed = true;
	// }
	// if (isUndoing) {
	// 	undo();
	// 	isRedoUndoed = true;
	// }
	return eventHandled;
}
