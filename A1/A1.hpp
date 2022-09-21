// Termm--Fall 2022

#pragma once

#include <glm/glm.hpp>

#include "cs488-framework/CS488Window.hpp"
#include "cs488-framework/OpenGLImport.hpp"
#include "cs488-framework/ShaderProgram.hpp"

#include "maze.hpp"

class A1 : public CS488Window {
public:
	A1();
	virtual ~A1();

protected:
	virtual void init() override;
	virtual void appLogic() override;
	virtual void guiLogic() override;
	virtual void draw() override;
	virtual void cleanup() override;

	virtual bool cursorEnterWindowEvent(int entered) override;
	virtual bool mouseMoveEvent(double xPos, double yPos) override;
	virtual bool mouseButtonInputEvent(int button, int actions, int mods) override;
	virtual bool mouseScrollEvent(double xOffSet, double yOffSet) override;
	virtual bool windowResizeEvent(int width, int height) override;
	virtual bool keyInputEvent(int key, int action, int mods) override;

private:
	void initGrid();
	void initCube();
	void initGround();

	void reset();

	Maze* m_ptr = nullptr;

	// Fields related to the shader and uniforms.
	ShaderProgram m_shader;
	GLint P_uni; // Uniform location for Projection matrix.
	GLint V_uni; // Uniform location for View matrix.
	GLint M_uni; // Uniform location for Model matrix.
	GLint col_uni;   // Uniform location for cube colour.

	// Fields related to grid geometry.
	GLuint m_grid_vao; // Vertex Array Object
	GLuint m_grid_vbo; // Vertex Buffer Object

	// Fields related to cube geometry.
	GLuint m_cube_vao;
	GLuint m_cube_vbo;
	GLuint m_cube_ebo;

	//Fields related to ground geometry.
	GLuint m_ground_vao;
	GLuint m_ground_vbo;

	// Mouse event
	bool clicked = false;
	float camera_rotation = 0.0f;
	float camera_scale = 1.0f;
	double preXPos = 0.0;

	float recordClickX;
	float recordReleaseX;

	float persistence = 0;

	// Matrices controlling the camera and projection.
	glm::mat4 proj;
	glm::mat4 view;

	// Model adjust

	float colour[3];
	float playerColour[3];
	float groundColour[3];
	int current_col;
};
