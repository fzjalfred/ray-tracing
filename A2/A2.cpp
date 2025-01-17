// Termm--Fall 2022

#include "A2.hpp"
#include "cs488-framework/GlErrorCheck.hpp"

#include <iostream>
using namespace std;

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
#include <algorithm>
using namespace glm;

#include "utils.hpp"
const float PI = 3.14159265f;


float cameraScale = 5;
float translationRatio = 76.0f;

double FOV = 30;
float near = 1.0f;
float far = -2.0f;


vec4 viewXaxis = vec4(-1,0,0,0);
vec4 viewYaxis = vec4(0,1,0,0);
vec4 viewZaxis = vec4(0,0,-1,0);
vec4 viewOrigin = vec4(0,0,1,1);

vec4 modelXaxis = vec4(1,0,0,0);
vec4 modelYaxis = vec4(0,1,0,0);
vec4 modelZaxis = vec4(0,0,1,0);
vec4 modelOrigin = vec4(0,0,0,1);


vec2 windowStart = vec2(-0.95, 0.95);
vec2 windowEnd = vec2(0.95, -0.95);

vec3 upP = vec3(windowStart, 0);
vec3 upN = vec3(vec2(windowStart[0], windowEnd[1]) - windowStart, 0);
vec3 leftP = vec3(windowStart, 0);
vec3 leftN = vec3(vec2(windowEnd[0], windowStart[1]) - windowStart, 0);
vec3 downP = vec3(windowEnd, 0);
vec3 downN = -upN;
vec3 rightP = vec3(windowEnd, 0);
vec3 rightN = -leftN;


void updateClippingConfig() {
	// upP = windowStart;
	// upN = vec2(windowStart[0], windowEnd[1]) - windowStart;
	// leftP = windowStart;
	// leftN = vec2(windowEnd[0], windowStart[1]) - windowStart;
	// downP = windowEnd;
	// downN = -upN;
	// rightP = windowEnd;
	// rightN = -leftN;
	upP = vec3(windowStart, 0);
	upN = vec3(vec2(windowStart[0], windowEnd[1]) - windowStart, 0);
	leftP = vec3(windowStart, 0);
	leftN = vec3(vec2(windowEnd[0], windowStart[1]) - windowStart, 0);
	downP = vec3(windowEnd, 0);
	downN = -upN;
	rightP = vec3(windowEnd, 0);
	rightN = -leftN;
}


//----------------------------------------------------------------------------------------
// Constructor
VertexData::VertexData()
	: numVertices(0),
	  index(0)
{
	positions.resize(kMaxVertices);
	colours.resize(kMaxVertices);
}


//----------------------------------------------------------------------------------------
// Constructor
A2::A2()
	: m_currentLineColour(vec3(0.0f))
{

}

//----------------------------------------------------------------------------------------
// Destructor
A2::~A2()
{
	
}

//----------------------------------------------------------------------------------------
/*
 * Called once, at program start.
 */
void A2::init()
{
	// Set the background colour.
	glClearColor(0.2, 0.5, 0.3, 1.0);

	createShaderProgram();

	glGenVertexArrays(1, &m_vao);

	enableVertexAttribIndices();

	generateVertexBuffers();

	mapVboDataToVertexAttributeLocation();

	model = mat4(1.0f);
	modelScale = mat4(1.0f);

	this->windowMatrix = mat4(1.0f);

	windowBefore = mat4(1.0f);
	windowBefore[0][0] = 0.2f;
	windowBefore[1][1] = 0.2f;
	windowBefore[2][2] = 0.2f;

	view = mylookAt( 
		glm::vec3( 0.0f, 0.0f, 1.0f ),
		glm::vec3( 0.0f, 0.0f, 0.0f ),
		glm::vec3( 0.0f, 1.0f, 0.0f ) );

	project = myPerspectiveDownZ(float( m_framebufferWidth ) / float( m_framebufferHeight ), glm::radians(FOV), near, far);
	// project = mat4(1.0f);
	// project = glm::perspective( 
	// 	glm::radians( 30.0f ),
	// 	float( m_framebufferWidth ) / float( m_framebufferHeight ),
	// 	-10.0f, -20.0f );
}

void A2::reset() {
	model = mat4(1.0f);
	view = mylookAt( 
		glm::vec3( 0.0f, 0.0f, 1.0f ),
		glm::vec3( 0.0f, 0.0f, 0.0f ),
		glm::vec3( 0.0f, 1.0f, 0.0f ) );

	worldFrame = {vec4(0,0,0,1), vec4(1,0,0,1), vec4(0,1,0,1), vec4(0,0,1,1)};
	objectFrame = {vec4(0,0,0,1), vec4(1,0,0,1), vec4(0,1,0,1), vec4(0,0,1,1)};
	
	viewXaxis = vec4(-1,0,0,0);
	viewYaxis = vec4(0,1,0,0);
	viewZaxis = vec4(0,0,-1,0);
	viewOrigin = vec4(0,0,1,1);

	modelXaxis = vec4(1,0,0,0);
	modelYaxis = vec4(0,1,0,0);
	modelZaxis = vec4(0,0,1,0);
	modelOrigin = vec4(0,0,0,1);

	windowStart = vec2(-0.95, 0.95);
	windowEnd = vec2(0.95, -0.95);
	windowMatrix = mat4(1.0f);

	far = -2.0f;
	near = 1.0f;
	modelScale = mat4(1.0f);
}

mat4 A2::mylookAt(vec3  const & eye, vec3  const & center, vec3  const & up) {
	
    vec3  f = normalize(center - eye); // camera Z-axis
    vec3  u = normalize(up); // camera Y-axis
    vec3  s = normalize(cross(f, u)); // camera X-axis
    u = cross(s, f);

    mat4 res(1.0f);
    res[0][0] = s.x;
    res[1][0] = s.y;
    res[2][0] = s.z;
    res[0][1] = u.x;
    res[1][1] = u.y;
    res[2][1] = u.z;
    res[0][2] =-f.x;
    res[1][2] =-f.y;
    res[2][2] =-f.z;
    res[3][0] =-dot(s, eye);
    res[3][1] =-dot(u, eye);
    res[3][2] = dot(f, eye);

    return res;

}

//----------------------------------------------------------------------------------------
void A2::createShaderProgram()
{
	m_shader.generateProgramObject();
	m_shader.attachVertexShader( getAssetFilePath("VertexShader.vs").c_str() );
	m_shader.attachFragmentShader( getAssetFilePath("FragmentShader.fs").c_str() );
	m_shader.link();
}

//---------------------------------------------------------------------------------------- Fall 2022
void A2::enableVertexAttribIndices()
{
	glBindVertexArray(m_vao);

	// Enable the attribute index location for "position" when rendering.
	GLint positionAttribLocation = m_shader.getAttribLocation( "position" );
	glEnableVertexAttribArray(positionAttribLocation);

	// Enable the attribute index location for "colour" when rendering.
	GLint colourAttribLocation = m_shader.getAttribLocation( "colour" );
	glEnableVertexAttribArray(colourAttribLocation);

	// Restore defaults
	glBindVertexArray(0);

	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
void A2::generateVertexBuffers()
{
	// Generate a vertex buffer to store line vertex positions
	{
		glGenBuffers(1, &m_vbo_positions);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_positions);

		// Set to GL_DYNAMIC_DRAW because the data store will be modified frequently.
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * kMaxVertices, nullptr,
				GL_DYNAMIC_DRAW);


		// Unbind the target GL_ARRAY_BUFFER, now that we are finished using it.
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		CHECK_GL_ERRORS;
	}

	// Generate a vertex buffer to store line colors
	{
		glGenBuffers(1, &m_vbo_colours);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_colours);

		// Set to GL_DYNAMIC_DRAW because the data store will be modified frequently.
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * kMaxVertices, nullptr,
				GL_DYNAMIC_DRAW);


		// Unbind the target GL_ARRAY_BUFFER, now that we are finished using it.
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		CHECK_GL_ERRORS;
	}
}

//----------------------------------------------------------------------------------------
void A2::mapVboDataToVertexAttributeLocation()
{
	// Bind VAO in order to record the data mapping.
	glBindVertexArray(m_vao);

	// Tell GL how to map data from the vertex buffer "m_vbo_positions" into the
	// "position" vertex attribute index for any bound shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_positions);
	GLint positionAttribLocation = m_shader.getAttribLocation( "position" );
	glVertexAttribPointer(positionAttribLocation, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Tell GL how to map data from the vertex buffer "m_vbo_colours" into the
	// "colour" vertex attribute index for any bound shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_colours);
	GLint colorAttribLocation = m_shader.getAttribLocation( "colour" );
	glVertexAttribPointer(colorAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	//-- Unbind target, and restore default values:
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	CHECK_GL_ERRORS;
}

//---------------------------------------------------------------------------------------
void A2::initLineData()
{
	m_vertexData.numVertices = 0;
	m_vertexData.index = 0;
}

//---------------------------------------------------------------------------------------
void A2::setLineColour (
		const glm::vec3 & colour
) {
	m_currentLineColour = colour;
}

//---------------------------------------------------------------------------------------
void A2::drawLine(
		const glm::vec2 & V0,   // Line Start (NDC coordinate)
		const glm::vec2 & V1    // Line End (NDC coordinate)
) {
	vec2 A = V0;
	vec2 B = V1;
	// if (!clipWindow(A, B)) {
	// 	return;
	// }

	m_vertexData.positions[m_vertexData.index] = A;
	m_vertexData.colours[m_vertexData.index] = m_currentLineColour;
	++m_vertexData.index;
	m_vertexData.positions[m_vertexData.index] = B;
	m_vertexData.colours[m_vertexData.index] = m_currentLineColour;
	++m_vertexData.index;

	m_vertexData.numVertices += 2;
}

void A2::drawLine(
		const glm::vec3 & V0,   // Line Start (NDC coordinate)
		const glm::vec3 & V1    // Line End (NDC coordinate)
) {
	vec3 A = V0;
	vec3 B = V1;
	if (!clipWindow(A, B)) {
		return;
	}

	m_vertexData.positions[m_vertexData.index] = vec2(A);
	m_vertexData.colours[m_vertexData.index] = m_currentLineColour;
	++m_vertexData.index;
	m_vertexData.positions[m_vertexData.index] = vec2(B);
	m_vertexData.colours[m_vertexData.index] = m_currentLineColour;
	++m_vertexData.index;

	m_vertexData.numVertices += 2;
}

bool A2::clipWindow(vec3& A, vec3& B) {
	// vec3 upP = vec3(windowStart, 0);
	// vec3 upN = vec3(::upN, 0);
	// vec3 leftP = vec3(::leftP, 0);
	// vec3 leftN = vec3(::leftN, 0);
	// vec3 downP = vec3(::downP, 0);;
	// vec3 downN = vec3(::downN, 0);
	// vec3 rightP = vec3(::rightP, 0);
	// vec3 rightN = vec3(::rightN, 0);
	if (clip(A, B, upP, upN) && clip(A, B, downP, downN) 
	&& clip(A, B, leftP, leftN) && clip(A, B, rightP, rightN)
	&& clip(A, B, vec3(0,0,far), vec3(0,0,1)) && clip(A, B, vec3(0,0,near), vec3(0,0,-1))) {
		return true;
	}
	return false;
	// if (clip(up)&&clip(down)&&clip(left)&&clip(right))
}


void A2::drawObjectFrame() {
	std::vector<vec4> objectFrame2d = {vec4(0,0,0,1), vec4(1,0,0,1), vec4(0,1,0,1), vec4(0,0,1,1)};
	for (int i = 0; i<objectFrame2d.size(); i++) {
		auto p = view*model*objectFrame[i];
		vec4 pos2d = project*windowBefore*p;
		pos2d/=pos2d[3];
		pos2d[2] = p[2];
		objectFrame2d[i] = windowMatrix*pos2d;
		// std::cout<<"x:"<< pos2d[0]<<" y:"<<pos2d[1]<<" z:"<<pos2d[2]<<std::endl;
	}
	setLineColour(vec3(0.5f, 0.5f, 1.0f));
	drawLine(vec3(objectFrame2d[0]), vec3(objectFrame2d[1]));
	setLineColour(vec3(0.5f, 1.0f, 1.5f));
	drawLine(vec3(objectFrame2d[0]), vec3(objectFrame2d[2]));
	setLineColour(vec3(1.0f, 0.5f, 1.5f));
	drawLine(vec3(objectFrame2d[0]), vec3(objectFrame2d[3]));
}

void A2::drawWorldFrame() {
	std::vector<vec4> worldFrame2d = {vec4(0,0,0,1), vec4(1,0,0,1), vec4(0,1,0,1), vec4(0,0,1,1)};
	for (int i = 0; i<worldFrame2d.size(); i++) {
		auto p = view*worldFrame[i];
		vec4 pos2d = project*windowBefore*p;
		pos2d/=pos2d[3];
		pos2d[2] = p[2];
		worldFrame2d[i] = windowMatrix*pos2d;
		// std::cout<<"x:"<< pos2d[0]<<" y:"<<pos2d[1]<<" z:"<<pos2d[2]<<std::endl;
	}
	setLineColour(vec3(0.0f, 0.0f, 1.0f));
	drawLine(vec3(worldFrame2d[0]), vec3(worldFrame2d[1]));
	setLineColour(vec3(0.0f, 1.0f, 1.0f));
	drawLine(vec3(worldFrame2d[0]), vec3(worldFrame2d[2]));
	setLineColour(vec3(1.0f, 0.0f, 1.0f));
	drawLine(vec3(worldFrame2d[0]), vec3(worldFrame2d[3]));
}


void A2::drawCube() {
	// cube.simpleProj(view, model);

	for (int i = 0; i<8; i++) {
		auto p = view*model*modelScale*cube.vertices[i];
		vec4 pos2d = project*windowBefore*p;
		pos2d/=pos2d[3];
		pos2d[2] = p[2];
		cube.vertices2d[i] = windowMatrix*pos2d;
		// std::cout<<"x:"<< pos2d[0]<<" y:"<<pos2d[1]<<" z:"<<pos2d[2]<<std::endl;
	}
	std::vector<vec4> verts = cube.vertices2d;
	// std::for_each(verts.begin(), verts.end(), [=](vec4 &c){c /= cameraScale;  });

	setLineColour(vec3(1.0f, 1.0f, 1.0f));

	drawLine(vec3(verts[0][0], verts[0][1], verts[0][2]), vec3(verts[1][0], verts[1][1], verts[1][2]));
	drawLine(vec3(verts[1][0], verts[1][1], verts[1][2]), vec3(verts[2][0], verts[2][1], verts[2][2]));
	//std::cout<<"("<<verts[1][0]<<", "<<verts[1][1]<<") " <<"("<<verts[2][0]<<", "<<verts[2][1]<<") "<<endl;
	drawLine(vec3(verts[2][0], verts[2][1], verts[2][2]), vec3(verts[3][0], verts[3][1], verts[3][2]));
	drawLine(vec3(verts[3][0], verts[3][1], verts[3][2]), vec3(verts[0][0], verts[0][1], verts[0][2]));

	drawLine(vec3(verts[0][0], verts[0][1], verts[0][2]), vec3(verts[4][0], verts[4][1], verts[4][2]));
	drawLine(vec3(verts[1][0], verts[1][1], verts[1][2]), vec3(verts[5][0], verts[5][1], verts[5][2]));
	drawLine(vec3(verts[2][0], verts[2][1], verts[2][2]), vec3(verts[6][0], verts[6][1], verts[6][2]));
	drawLine(vec3(verts[3][0], verts[3][1], verts[3][2]), vec3(verts[7][0], verts[7][1], verts[7][2]));

	drawLine(vec3(verts[4][0], verts[4][1], verts[4][2]), vec3(verts[5][0], verts[5][1], verts[5][2]));
	drawLine(vec3(verts[5][0], verts[5][1], verts[5][2]), vec3(verts[6][0], verts[6][1], verts[6][2]));
	drawLine(vec3(verts[6][0], verts[6][1], verts[6][2]), vec3(verts[7][0], verts[7][1], verts[7][2]));
	drawLine(vec3(verts[7][0], verts[7][1], verts[7][2]), vec3(verts[4][0], verts[4][1], verts[4][2]));
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, before guiLogic().
 */
void A2::appLogic()
{
	// Place per frame, application logic here ...

	// Call at the beginning of frame, before drawing lines:
	initLineData();

	// Draw outer square:
	setLineColour(vec3(1.0f, 0.7f, 0.8f));
	drawLine(vec2(windowStart[0], windowEnd[1]), windowEnd);
	drawLine(windowEnd, vec2(windowEnd[0], windowStart[1]));
	drawLine(vec2(windowEnd[0], windowStart[1]), windowStart);
	drawLine(windowStart, vec2(windowStart[0], windowEnd[1]));
	//drawLine(vec2(-0.95f, -0.95f), vec2(0.95f, -0.95f));
	//drawLine(vec2(0.95f, -0.95f), vec2(0.95f, 0.95f));
	//drawLine(vec2(0.95f, 0.95f), vec2(-0.95f, 0.95f));
	//drawLine(vec2(-0.95f, 0.95f), vec2(-0.95f, -0.95f));
	updateClippingConfig();


	// // Draw inner square:
	// setLineColour(vec3(0.2f, 1.0f, 1.0f));
	// drawLine(vec2(-0.25f, -0.25f), vec2(0.25f, -0.25f));
	// drawLine(vec2(0.25f, -0.25f), vec2(0.25f, 0.25f));
	// drawLine(vec2(0.25f, 0.25f), vec2(-0.25f, 0.25f));
	// drawLine(vec2(-0.25f, 0.25f), vec2(-0.25f, -0.25f));

	drawCube();
	//*** draw view frame for debug ***//
	//drawFrame(vector<vec4> {viewOrigin, viewOrigin+viewXaxis, viewOrigin+viewYaxis, viewOrigin+viewZaxis});
	drawObjectFrame();
	drawWorldFrame();
	//*** draw midpoint of window for debug ***//
	// drawLine(vec2(0,0), vec2(windowMatrix[3][0], windowMatrix[3][1]));
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after appLogic(), but before the draw() method.
 */
void A2::guiLogic()
{
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
		if( ImGui::RadioButton( "Rotation View (O)", &interaction, 0 ) ) {
			// Select this colour.
			interaction = RotateView;
		}
		if( ImGui::RadioButton( "Translate View (E)", &interaction, 1 ) ) {
			// Select this colour.
			interaction = TranslateView;
		}
		if( ImGui::RadioButton( "Perspective (P)", &interaction, 2 ) ) {
			// Select this colour.
			interaction = Perspective;
		}
		if( ImGui::RadioButton( "Rotate Model (R)", &interaction, 3 ) ) {
			// Select this colour.
			interaction = RotateModel;
		}
		if( ImGui::RadioButton( "Translate Model (T)", &interaction, 4 ) ) {
			// Select this colour.
			interaction = TranslateModel;
		}
		if( ImGui::RadioButton( "Scale Model (S)", &interaction, 5 ) ) {
			// Select this colour.
			interaction = ScaleModel;
		}
		if( ImGui::RadioButton( "Viewport (V)", &interaction, 6 ) ) {
			// Select this colour.
			interaction = Viewport;
		}

		if( ImGui::Button( "Reset(A)" ) ) {
			reset();
		}

		// Create Button, and check if it was clicked:
		if( ImGui::Button( "Quit Application(Q)" ) ) {
			glfwSetWindowShouldClose(m_window, GL_TRUE);
		}

		ImGui::Text( "Framerate: %.1f FPS", ImGui::GetIO().Framerate );
		ImGui::Text( "NearPlane: %.1f ", near );
		ImGui::Text( "FarPlane: %.1f ", far );

	ImGui::End();
}

//----------------------------------------------------------------------------------------
void A2::uploadVertexDataToVbos() {

	//-- Copy vertex position data into VBO, m_vbo_positions:
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_positions);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec2) * m_vertexData.numVertices,
				m_vertexData.positions.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		CHECK_GL_ERRORS;
	}

	//-- Copy vertex colour data into VBO, m_vbo_colours:
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_colours);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3) * m_vertexData.numVertices,
				m_vertexData.colours.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		CHECK_GL_ERRORS;
	}
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after guiLogic().
 */
void A2::draw()
{
	uploadVertexDataToVbos();

	glBindVertexArray(m_vao);

	m_shader.enable();
		glDrawArrays(GL_LINES, 0, m_vertexData.numVertices);
	m_shader.disable();

	// Restore defaults
	glBindVertexArray(0);

	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
/*
 * Called once, after program is signaled to terminate.
 */
void A2::cleanup()
{

}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles cursor entering the window area events.
 */
bool A2::cursorEnterWindowEvent (
		int entered
) {
	bool eventHandled(false);

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse cursor movement events.
 */
bool A2::mouseMoveEvent (
		double xPos,
		double yPos
) {
	bool eventHandled(false);

	if (!ImGui::IsMouseHoveringAnyWindow()) {
		// Put some code here to handle rotations.  Probably need to
		// check whether we're *dragging*, not just moving the mouse.
		// Probably need some instance variables to track the current
		// rotation amount, and maybe the previous X position (so 
		// that you can rotate relative to the *change* in X.
		switch(interaction) {
			case RotateView:
				if (rightMousePressed) {
					double camera_rotation = xPos - preXPos;
					double angle = camera_rotation/90;
					// mat4 rotationZ = glm::mat4(1.0f);
					// rotationZ[0][0] = cos(angle);
					// rotationZ[0][1] = -sin(angle);
					// rotationZ[1][0] = sin(angle);
					// rotationZ[1][1] = cos(angle);
					mat4 transBack = mat4(1);
					transBack[3][0] = -viewOrigin[0];
					transBack[3][1] = -viewOrigin[1];
					transBack[3][2] = -viewOrigin[2];

					mat4 rotationZ = myRotate(angle, viewZaxis);
					mat4 combine = inverse(transBack)*rotationZ*transBack;
					viewXaxis = inverse(combine)*viewXaxis;
					viewYaxis = inverse(combine)*viewYaxis;
					view *= combine;
				}
				if (leftMousePressed) {
					double camera_rotation = xPos - preXPos;
					double angle = camera_rotation/90;
					// mat4 rotationX = glm::mat4(1.0f);
					// rotationX[1][1] = cos(angle);
					// rotationX[1][2] = -sin(angle);
					// rotationX[2][1] = sin(angle);
					// rotationX[2][2] = cos(angle);
					mat4 transBack = mat4(1);
					transBack[3][0] = -viewOrigin[0];
					transBack[3][1] = -viewOrigin[1];
					transBack[3][2] = -viewOrigin[2];
					mat4 rotationX = myRotate(angle, viewXaxis);
					mat4 combine = inverse(transBack)*rotationX*transBack;
					// viewXaxis = combine*viewXaxis;
					viewYaxis = inverse(combine)*viewYaxis;
					viewZaxis = inverse(combine)*viewZaxis;
					view *= combine;
				}
				if (middleMousePressed) {
					double camera_rotation = xPos - preXPos;
					double angle = camera_rotation/90;
					// mat4 rotationY = glm::mat4(1.0f);
					// rotationY[0][0] = cos(angle);
					// rotationY[0][2] = -sin(angle);
					// rotationY[2][0] = sin(angle);
					// rotationY[2][2] = cos(angle);
					mat4 transBack = mat4(1);
					transBack[3][0] = -viewOrigin[0];
					transBack[3][1] = -viewOrigin[1];
					transBack[3][2] = -viewOrigin[2];
					mat4 rotationY = myRotate(angle, viewYaxis);
					mat4 combine = inverse(transBack)*rotationY*transBack;
					viewXaxis = inverse(combine)*viewXaxis;
					viewZaxis = inverse(combine)*viewZaxis;
					view *= combine;
				}
				break;
			case TranslateView: {
					mat4 translate = mat4(1.0f);
					double xTrans = xPos - preXPos;
					double yTrans = yPos - preYPos;
					if (rightMousePressed) {
						translate[3][2] = -xTrans/translationRatio;
						viewOrigin[2] += xTrans/translationRatio;
					}
					if (leftMousePressed) {
						translate[3][0] = -xTrans/translationRatio;
						viewOrigin[0] += xTrans/translationRatio;
					}
					if (middleMousePressed) {
						translate[3][1] = -xTrans/translationRatio;
						viewOrigin[1] += xTrans/translationRatio;
					}
					view *= translate;
				}
				break;
			case Perspective:
				{
					mat4 scale = mat4(1.0f);
					double xTrans = xPos - preXPos;
					if (leftMousePressed) {
						FOV += xTrans/translationRatio;
						if (FOV < 5) {
							FOV = 5;
						}
						if (FOV > 160) {
							FOV = 160;
						}
					}
					if (middleMousePressed) {
						if (near + xTrans/translationRatio >= far) {
							near += xTrans/translationRatio;
						}
						if (near > 10.0f) {
							near = 10.0f;
						}
					}
					if (rightMousePressed) {
						if (far + xTrans/translationRatio <= near) {
							far += xTrans/translationRatio;
						}
						if (far < -20.0f) {
							far = -20.0f;
						}
						
					}
					// cout << "near: "<<near<<endl;
					// cout << "far: "<<far<<endl;
					project = myPerspectiveDownZ(float( m_framebufferWidth ) / float( m_framebufferHeight ), glm::radians(FOV), near, far);
					
				}
				break;
			case RotateModel:
				if (rightMousePressed) {
					double camera_rotation = xPos - preXPos;
					double angle = camera_rotation/90;
					mat4 rotationZ = glm::mat4(1.0f);
					rotationZ[0][0] = cos(angle);
					rotationZ[0][1] = -sin(angle);
					rotationZ[1][0] = sin(angle);
					rotationZ[1][1] = cos(angle);
					model *= rotationZ;
				}
				if (leftMousePressed) {
					double camera_rotation = xPos - preXPos;
					double angle = camera_rotation/90;
					mat4 rotationX = glm::mat4(1.0f);
					rotationX[1][1] = cos(angle);
					rotationX[1][2] = -sin(angle);
					rotationX[2][1] = sin(angle);
					rotationX[2][2] = cos(angle);
					model *= rotationX;
				}
				if (middleMousePressed) {
					double camera_rotation = xPos - preXPos;
					double angle = camera_rotation/90;
					mat4 rotationY = glm::mat4(1.0f);
					rotationY[0][0] = cos(angle);
					rotationY[0][2] = -sin(angle);
					rotationY[2][0] = sin(angle);
					rotationY[2][2] = cos(angle);
					model *= rotationY;
				}
				break;
			case TranslateModel:
				{
					mat4 translate = mat4(1.0f);
					double xTrans = xPos - preXPos;
					double yTrans = yPos - preYPos;
					if (rightMousePressed) {
						translate[3][2] = +xTrans/translationRatio;
						modelOrigin[2] += xTrans/translationRatio;
					}
					if (leftMousePressed) {
						translate[3][0] = +xTrans/translationRatio;
						modelOrigin[0] += xTrans/translationRatio;
					}
					if (middleMousePressed) {
						translate[3][1] = +xTrans/translationRatio;
						modelOrigin[1] += xTrans/translationRatio;
					}
					model *= translate;
				}
				break;
			case ScaleModel: 
				{
					mat4 scale = mat4(1.0f);
					float scaleRatio = 1.0f + (xPos - preXPos)/translationRatio;
					if (leftMousePressed) {
						scale[0][0] = scaleRatio;
					}
					if (middleMousePressed) {
						scale[1][1] = scaleRatio;
					}
					if (rightMousePressed) {
						scale[2][2] = scaleRatio;
					}
					modelScale *= scale;
				}
				break;
			case Viewport:
				if (viewResizing) {
					mat4 scale = mat4(1.0f);
					double xTrans = xPos - preXPos;
					if (leftMousePressed) {
						windowEnd[0] = xPos/m_windowWidth*2 + -1;
						windowEnd[1] = 1 - yPos/m_windowHeight*2;
						windowMatrix[0][0] = (windowEnd[0] - windowStart[0])/1.9f;
						windowMatrix[1][1] = (windowStart[1] - windowEnd[1])/1.9f;
						//x translation
						windowMatrix[3][0] = (windowEnd[0] + windowStart[0])/2.0f;
						windowMatrix[3][1] = (windowEnd[1] + windowStart[1])/2.0f;
					}
					if (middleMousePressed) {
						
					}
					if (rightMousePressed) {
						
					}
				}
				break;
		}
		
		preYPos = yPos;
		preXPos = xPos;
	}

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse button events.
 */
bool A2::mouseButtonInputEvent (
		int button,
		int actions,
		int mods
) {
	bool eventHandled(false);

	// Fill in with event handling code...
	if (!ImGui::IsMouseHoveringAnyWindow()) {
		// The user clicked in the window.  If it's the left
		// mouse button, initiate a rotation.
		if (button == GLFW_MOUSE_BUTTON_1 && actions == GLFW_PRESS) {
			leftMousePressed = true;
			if (interaction == Viewport) {
				viewResizing = true;
				windowStart[0] = preXPos/m_windowWidth*2 + -1;
				windowStart[1] = 1 - preYPos/m_windowHeight*2;
			}
		}
		if (button == GLFW_MOUSE_BUTTON_1 && actions == GLFW_RELEASE) {
			leftMousePressed = false;
			viewResizing = false;
		}
		if (button == GLFW_MOUSE_BUTTON_2 && actions == GLFW_PRESS) {
			rightMousePressed = true;
		}
		if (button == GLFW_MOUSE_BUTTON_2 && actions == GLFW_RELEASE) {
			rightMousePressed = false;
		}
		if (button == GLFW_MOUSE_BUTTON_3 && actions == GLFW_PRESS) {
			middleMousePressed = true;
		}
		if (button == GLFW_MOUSE_BUTTON_3 && actions == GLFW_RELEASE) {
			middleMousePressed = false;
		}
		if (interaction == Viewport && button == GLFW_MOUSE_BUTTON_1 && actions == GLFW_PRESS) {
			viewResizing = true;
			
		}
	}

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse scroll wheel events.
 */
bool A2::mouseScrollEvent (
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
bool A2::windowResizeEvent (
		int width,
		int height
) {
	bool eventHandled(false);

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles key input events.
 */
bool A2::keyInputEvent (
		int key,
		int action,
		int mods
) {
	bool eventHandled(false);

	// Fill in with event handling code...
	if( action == GLFW_PRESS ) {
		// Respond to some key events.
		if (key == GLFW_KEY_Q) {
			glfwSetWindowShouldClose(m_window, GL_TRUE);
		}
		if (key == GLFW_KEY_A) {
			reset();
		}
		if (key == GLFW_KEY_O) {
			interaction = RotateView;
		}
		if (key == GLFW_KEY_E) {
			interaction = TranslateView;
		}
		if (key == GLFW_KEY_P) {
			interaction = Perspective;
		}
		if (key == GLFW_KEY_R) {
			interaction = RotateModel;
		}
		if (key == GLFW_KEY_T) {
			interaction = TranslateModel;
		}
		if (key == GLFW_KEY_S) {
			interaction = ScaleModel;
		}
		if (key == GLFW_KEY_V) {
			interaction = Viewport;
		}
	}
	return eventHandled;
}
