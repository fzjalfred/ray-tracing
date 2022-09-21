// Termm--Fall 2022

#include "A1.hpp"
#include "cs488-framework/GlErrorCheck.hpp"

#include <iostream>

#include <sys/types.h>
#include <unistd.h>

#include <imgui/imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Cube.hpp"

using namespace glm;
using namespace std;

static const size_t DIM = 16;

glm::vec3 cameraPos   = glm::vec3( 0.0f, 2.*float(DIM)*2.0*M_SQRT1_2, float(DIM)*2.0*M_SQRT1_2 );
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
GLfloat camX = sin(0);
GLfloat camZ = cos(0);
glm::vec3 movefoward = cameraFront - cameraPos;

float cubes_colour[3] = {0, 0.3, 0.6};
float ground_colour[3] = {0.75, 0.67,0.5};
float player_colour[3] = {1.0, 0.2,1.0};
float height_scale = 1.0f;

//----------------------------------------------------------------------------------------
// Constructor
A1::A1()
	: current_col( 0 )
{
	colour[0] = 0.0f;
	colour[1] = 0.0f;
	colour[2] = 0.0f;
}

//----------------------------------------------------------------------------------------
// Destructor
A1::~A1()
{
	delete m_ptr;
}

//----------------------------------------------------------------------------------------
/*
 * Called once, at program start.
 */
void A1::init()
{
	// Initialize random number generator
	int rseed=getpid();
	srandom(rseed);
	// Print random number seed in case we want to rerun with
	// same random numbers
	cout << "Random number seed = " << rseed << endl;
	

	// DELETE FROM HERE...
	m_ptr = new Maze(DIM);
	m_ptr->digMaze();
	m_ptr->printMaze();
	// ...TO HERE
	
	// Set the background colour.
	glClearColor( 0.3, 0.5, 0.7, 1.0 );

	// Build the shader
	m_shader.generateProgramObject();
	m_shader.attachVertexShader(
		getAssetFilePath( "VertexShader.vs" ).c_str() );
	m_shader.attachFragmentShader(
		getAssetFilePath( "FragmentShader.fs" ).c_str() );
	m_shader.link();

	// Set up the uniforms
	P_uni = m_shader.getUniformLocation( "P" );
	V_uni = m_shader.getUniformLocation( "V" );
	M_uni = m_shader.getUniformLocation( "M" );
	col_uni = m_shader.getUniformLocation( "colour" );

	initGrid();
	initGround();
	colour[0] = cubes_colour[0];
	colour[1] = cubes_colour[1];
	colour[2] = cubes_colour[2];
	
	m_ptr->genMazeCubes(m_shader);

	// Set up initial view and projection matrices (need to do this here,
	// since it depends on the GLFW window being set up correctly).
	view = glm::lookAt( 
		glm::vec3( 0.0f, 2.*float(DIM)*2.0*M_SQRT1_2, float(DIM)*2.0*M_SQRT1_2 ),
		glm::vec3( 0.0f, 0.0f, 0.0f ),
		glm::vec3( 0.0f, 1.0f, 0.0f ) );

	proj = glm::perspective( 
		glm::radians( 30.0f ),
		float( m_framebufferWidth ) / float( m_framebufferHeight ),
		1.0f, 1000.0f );
}

void A1::initGround() {
	GLfloat vertices[] = {
    	0.0f, 0.0f, 0.0f,
     	1.0f*DIM, 0.0f, 0.0f,
     	1.0f*DIM, 0.0f, 1.0f*DIM,
		0.0f, 0.0f, 0.0f,
     	0.0f, 0.0f, 1.0f*DIM,
     	1.0f*DIM, 0.0f, 1.0f*DIM
	};
	// Create the vertex array to record buffer assignments.
	glGenVertexArrays( 1, &m_ground_vao );
	glBindVertexArray( m_ground_vao );

	// Create the cube vertex buffer
	glGenBuffers( 1, &m_ground_vbo );
	glBindBuffer( GL_ARRAY_BUFFER, m_ground_vbo );
	glBufferData( GL_ARRAY_BUFFER, sizeof(vertices),
		vertices, GL_STATIC_DRAW );

	// Specify the means of extracting the position values properly.
	GLint posAttrib = m_shader.getAttribLocation( "position" );
	glEnableVertexAttribArray( posAttrib );
	glVertexAttribPointer( posAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr );

	// Reset state to prevent rogue code from messing with *my* 
	// stuff!
	glBindVertexArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

void A1::initGrid()
{
	size_t sz = 3 * 2 * 2 * (DIM+3);

	float *verts = new float[ sz ];
	size_t ct = 0;
	for( int idx = 0; idx < DIM+3; ++idx ) {
		verts[ ct ] = -1;
		verts[ ct+1 ] = 0;
		verts[ ct+2 ] = idx-1;
		verts[ ct+3 ] = DIM+1;
		verts[ ct+4 ] = 0;
		verts[ ct+5 ] = idx-1;
		ct += 6;

		verts[ ct ] = idx-1;
		verts[ ct+1 ] = 0;
		verts[ ct+2 ] = -1;
		verts[ ct+3 ] = idx-1;
		verts[ ct+4 ] = 0;
		verts[ ct+5 ] = DIM+1;
		ct += 6;
	}

	// Create the vertex array to record buffer assignments.
	glGenVertexArrays( 1, &m_grid_vao );
	glBindVertexArray( m_grid_vao );

	// Create the cube vertex buffer
	glGenBuffers( 1, &m_grid_vbo );
	glBindBuffer( GL_ARRAY_BUFFER, m_grid_vbo );
	glBufferData( GL_ARRAY_BUFFER, sz*sizeof(float),
		verts, GL_STATIC_DRAW );

	// Specify the means of extracting the position values properly.
	GLint posAttrib = m_shader.getAttribLocation( "position" );
	glEnableVertexAttribArray( posAttrib );
	glVertexAttribPointer( posAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr );

	// Reset state to prevent rogue code from messing with *my* 
	// stuff!
	glBindVertexArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	// OpenGL has the buffer now, there's no need for us to keep a copy.
	delete [] verts;

	CHECK_GL_ERRORS;
}

void A1::initCube() {
	GLfloat vertices[] = {
    	0.0f, 0.0f, 0.0f,
     	1.0f, 0.0f, 0.0f,
     	1.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,

		0.0f, 0.0f, 1.0f,
     	1.0f, 0.0f, 1.0f,
     	1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f
	};

	GLuint indices[] = {
    	0, 1, 2,
		0, 2, 3,

		0, 1, 4,
		4, 5, 1,

		0, 4, 7,
		0, 3, 7,

		3, 7, 6,
		3, 2, 6,

		7, 6, 5,
		7, 4, 5,

		6, 2, 1,
		6, 5, 1
	};

	// Create the vertex array to record buffer assignments.
	glGenVertexArrays( 1, &m_cube_vao );
	glBindVertexArray( m_cube_vao );


	//VBO
	glGenBuffers(1, &m_cube_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_cube_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	//EBO
	glGenBuffers(1, &m_cube_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_cube_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	// Attribute
	GLint posAttrib = m_shader.getAttribLocation( "position" );
	glEnableVertexAttribArray( posAttrib );
	glVertexAttribPointer( posAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr );

	glBindVertexArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	

	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, before guiLogic().
 */
void A1::appLogic()
{
	if (current_col == 0) {
		Cube::updateColour(colour);
	}
	if (current_col == 1) {
		player_colour[0] = colour[0];
		player_colour[1] = colour[1];
		player_colour[2] = colour[2];
	}
	if (current_col == 2) {
		ground_colour[0] = colour[0];
		ground_colour[1] = colour[1];
		ground_colour[2] = colour[2];
	}
	
	// Place per frame, application logic here ...
}

void A1::reset() {
	height_scale = 1.0f;
	this->camera_rotation = 0.0f;
	this->camera_scale = 1.0f;
	this->view = glm::lookAt( 
		glm::vec3( 0.0f, 2.*float(DIM)*2.0*M_SQRT1_2, float(DIM)*2.0*M_SQRT1_2 ),
		glm::vec3( 0.0f, 0.0f, 0.0f ),
		glm::vec3( 0.0f, 1.0f, 0.0f ) );

	float cubes_colour[3] = {0, 0.3, 0.6};
	memcpy(::cubes_colour, cubes_colour, sizeof(cubes_colour)); 
	float ground_colour[3] = {0.75, 0.67,0.5};
	memcpy(::ground_colour, ground_colour, sizeof(ground_colour)); 
	float player_colour[3] = {1.0,0.2,1.0};
	memcpy(::player_colour, player_colour, sizeof(player_colour)); 
	if (current_col == 0) {
		memcpy(colour, cubes_colour, sizeof(cubes_colour));
	}
	if (current_col == 1) {
		memcpy(colour, player_colour, sizeof(cubes_colour));
	}
	if (current_col == 2) {
		memcpy(colour, ground_colour, sizeof(ground_colour)); 
	}
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after appLogic(), but before the draw() method.
 */
void A1::guiLogic()
{
	// We already know there's only going to be one window, so for 
	// simplicity we'll store button states in static local variables.
	// If there was ever a possibility of having multiple instances of
	// A1 running simultaneously, this would break; you'd want to make
	// this into instance fields of A1.
	static bool showTestWindow(false);
	static bool showDebugWindow(true);

	ImGuiWindowFlags windowFlags(ImGuiWindowFlags_AlwaysAutoResize);
	float opacity(0.5f);

	ImGui::Begin("Debug Window", &showDebugWindow, ImVec2(100,100), opacity, windowFlags);
		if( ImGui::Button( "Quit Application" ) ) {
			glfwSetWindowShouldClose(m_window, GL_TRUE);
		}

		if( ImGui::Button( "Reset" ) ) {
            reset();
        }

		if ( ImGui::Button( "Dig" ) ) {
			m_ptr->digMaze();
			m_ptr->genMazeCubes(m_shader);
		}

		// Eventually you'll create multiple colour widgets with
		// radio buttons.  If you use PushID/PopID to give them all
		// unique IDs, then ImGui will be able to keep them separate.
		// This is unnecessary with a single colour selector and
		// radio button, but I'm leaving it in as an example.

		// Prefixing a widget name with "##" keeps it from being
		// displayed.

		ImGui::PushID( 0 );
		ImGui::ColorEdit3( "##Colour", colour );
		ImGui::SameLine();
		if( ImGui::RadioButton( "Wall", &current_col, 0 ) ) {
			// Select this colour.
			colour[0] = cubes_colour[0];
			colour[1] = cubes_colour[1];
			colour[2] = cubes_colour[2];
		}
		ImGui::SameLine();
		if( ImGui::RadioButton( "Player", &current_col, 1 ) ) {
			// Select this colour.
			colour[0] = player_colour[0];
			colour[1] = player_colour[1];
			colour[2] = player_colour[2];
		}
		ImGui::SameLine();
		if( ImGui::RadioButton( "Ground", &current_col, 2 ) ) {
			// Select this colour.
			colour[0] = ground_colour[0];
			colour[1] = ground_colour[1];
			colour[2] = ground_colour[2];
		}
		ImGui::PopID();

/*
		// For convenience, you can uncomment this to show ImGui's massive
		// demonstration window right in your application.  Very handy for
		// browsing around to get the widget you want.  Then look in 
		// shared/imgui/imgui_demo.cpp to see how it's done.
		if( ImGui::Button( "Test Window" ) ) {
			showTestWindow = !showTestWindow;
		}
*/

		ImGui::Text( "Framerate: %.1f FPS", ImGui::GetIO().Framerate );

	ImGui::End();

	if( showTestWindow ) {
		ImGui::ShowTestWindow( &showTestWindow );
	}
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after guiLogic().
 */
void A1::draw()
{
	// Create a global transformation for the model (centre it).
	mat4 W;
	W = glm::translate( W, vec3( -float(DIM)/2.0f, 0, -float(DIM)/2.0f ) );

	glm::mat4 transform = glm::scale(mat4(), glm::vec3(1.0f, height_scale, 1.0f));

	W *= transform;

	m_shader.enable();
		glEnable( GL_DEPTH_TEST );

		glUniformMatrix4fv( P_uni, 1, GL_FALSE, value_ptr( proj ) );
		glUniformMatrix4fv( V_uni, 1, GL_FALSE, value_ptr( view ) );
		glUniformMatrix4fv( M_uni, 1, GL_FALSE, value_ptr( W ) );

		// Just draw the grid for now.
		glBindVertexArray( m_grid_vao );
		glUniform3f( col_uni, 1, 1, 1 );
		glDrawArrays( GL_LINES, 0, (3+DIM)*4 );

		glBindVertexArray( m_ground_vao);
		glUniform3f( col_uni, ground_colour[0], ground_colour[1], ground_colour[2] );
		glDrawArrays( GL_TRIANGLES, 0, 3*2 );

		m_ptr->drawMaze(col_uni);
		// Cube a = Cube(m_shader, (float)0, 0.0f, (float)0);
		// a.draw(col_uni);
		// Cube b = Cube(m_shader, (float)1, 0.0f, (float)0);
		// b.draw(col_uni);
		
		// Draw the cubes
		// Highlight the active square.
	m_shader.disable();

	// Restore defaults
	glBindVertexArray( 0 );

	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
/*
 * Called once, after program is signaled to terminate.
 */
void A1::cleanup()
{}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles cursor entering the window area events.
 */
bool A1::cursorEnterWindowEvent (
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
bool A1::mouseMoveEvent(double xPos, double yPos) 
{
	bool eventHandled(false);

	if (!ImGui::IsMouseHoveringAnyWindow()) {
		// Put some code here to handle rotations.  Probably need to
		// check whether we're *dragging*, not just moving the mouse.
		// Probably need some instance variables to track the current
		// rotation amount, and maybe the previous X position (so 
		// that you can rotate relative to the *change* in X.
		
		vec3 z_axis(0.0f,1.0f,0.0f);
		if (clicked) {
			camera_rotation = xPos - preXPos;
			view *= glm::rotate(mat4(), camera_rotation*0.01f, z_axis);
		}
		preXPos = xPos;
	}

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse button events.
 */
bool A1::mouseButtonInputEvent(int button, int actions, int mods) {
	bool eventHandled(false);

	if (!ImGui::IsMouseHoveringAnyWindow()) {
		// The user clicked in the window.  If it's the left
		// mouse button, initiate a rotation.
		if (button == GLFW_MOUSE_BUTTON_1 && actions == GLFW_PRESS) {
			clicked = true;
		}
		if (button == GLFW_MOUSE_BUTTON_1 && actions == GLFW_RELEASE) {
			clicked = false;
		}
	}

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse scroll wheel events.
 */
bool A1::mouseScrollEvent(double xOffSet, double yOffSet) {
	bool eventHandled(false);

	float cameraSpeed = 0.05f;
	// glm::vec3 movefoward = cameraFront - cameraPos;
	// Zoom in or out.
	camera_scale = 1.0f + cameraSpeed * yOffSet;
	view *= glm::scale(mat4(), vec3(camera_scale));

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles window resize events.
 */
bool A1::windowResizeEvent(int width, int height) {
	bool eventHandled(false);

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles key input events.
 */
bool A1::keyInputEvent(int key, int action, int mods) {
	bool eventHandled(false);

	// Fill in with event handling code...
	
	if( action == GLFW_PRESS ) {
		// Respond to some key events.
		if (key == GLFW_KEY_Q) {
			glfwSetWindowShouldClose(m_window, GL_TRUE);
		}
		if (key == GLFW_KEY_R) {
			reset();
		}
		if (key == GLFW_KEY_D) {
			m_ptr->digMaze();
			m_ptr->genMazeCubes(m_shader);
		}
		if ( key == GLFW_KEY_SPACE ) {
			if (height_scale + 0.5f <= 10.0f)
			height_scale += 0.5f;
			m_ptr->player->init();
		}
		if ( key == GLFW_KEY_BACKSPACE ) {
			height_scale -= 0.5f;
			m_ptr->player->init();
		}
		if (key == GLFW_KEY_UP) {
			if (mods == GLFW_MOD_SHIFT) {
				m_ptr->playerCheatMove(GLFW_KEY_UP);
			} else {
				m_ptr->playerMove(GLFW_KEY_UP);
			}
		}
		if (key == GLFW_KEY_DOWN) {
			if (mods == GLFW_MOD_SHIFT) {
				m_ptr->playerCheatMove(GLFW_KEY_DOWN);
			} else {
				m_ptr->playerMove(GLFW_KEY_DOWN);
			}
			
		}
		if (key == GLFW_KEY_LEFT) {
			if (mods == GLFW_MOD_SHIFT) {
				m_ptr->playerCheatMove(GLFW_KEY_LEFT);
			} else {
				m_ptr->playerMove(GLFW_KEY_LEFT);
			}
		}
		if (key == GLFW_KEY_RIGHT) {
			if (mods == GLFW_MOD_SHIFT) {
				m_ptr->playerCheatMove(GLFW_KEY_RIGHT);
			} else {
				m_ptr->playerMove(GLFW_KEY_RIGHT);
			}
		}
	}
	

	return eventHandled;
}
