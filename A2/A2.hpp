// Termm--Fall 2022

#pragma once

#include "cs488-framework/CS488Window.hpp"
#include "cs488-framework/OpenGLImport.hpp"
#include "cs488-framework/ShaderProgram.hpp"

#include <glm/glm.hpp>

#include <vector>
#include "Cube.hpp"

// Set a global maximum number of vertices in order to pre-allocate VBO data
// in one shot, rather than reallocating each frame.
const GLsizei kMaxVertices = 1000;


// Convenience class for storing vertex data in CPU memory.
// Data should be copied over to GPU memory via VBO storage before rendering.
class VertexData {
public:
	VertexData();

	std::vector<glm::vec2> positions;
	std::vector<glm::vec3> colours;
	GLuint index;
	GLsizei numVertices;
};


class A2 : public CS488Window {
private:
	Cube cube = Cube(-1, -1, -1, 2.0f);
	enum {
		RotateView,
		TranslateView,
		Perspective,
		RotateModel,
		TranslateModel,
		ScaleModel,
		Viewport
	};

public:
	A2();
	virtual ~A2();

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

	void createShaderProgram();
	void enableVertexAttribIndices();
	void generateVertexBuffers();
	void mapVboDataToVertexAttributeLocation();
	void uploadVertexDataToVbos();

	void initLineData();

	void setLineColour(const glm::vec3 & colour);

	void drawLine (
			const glm::vec2 & v0,
			const glm::vec2 & v1
	);
	void drawLine (
			const glm::vec3 & v0,
			const glm::vec3 & v1
	);

	ShaderProgram m_shader;

	GLuint m_vao;            // Vertex Array Object
	GLuint m_vbo_positions;  // Vertex Buffer Object
	GLuint m_vbo_colours;    // Vertex Buffer Object

	void reset();

	// interaction model
	int interaction;

	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 project = mat4(1.0f);
	glm::mat4 modelScale;
	glm::mat4 windowMatrix; 
	glm::mat4 windowBefore;
	mat4 mylookAt(vec3  const & eye, vec3  const & center, vec3  const & up);
	bool leftMousePressed;
	bool middleMousePressed;
	bool rightMousePressed;

	double preYPos;
	double preXPos;

	std::vector<vec4> worldFrame = {vec4(0,0,0,1), vec4(1,0,0,1), vec4(0,1,0,1), vec4(0,0,1,1)};
	std::vector<vec4> objectFrame = {vec4(0,0,0,1), vec4(1,0,0,1), vec4(0,1,0,1), vec4(0,0,1,1)};

	void drawWorldFrame();
	void drawObjectFrame();
	void drawCube();


	//Clipping window
	// bool clipWindow(vec2& A, vec2& B);
	bool clipWindow(vec3& A, vec3& B);
	bool viewResizing = false;


	VertexData m_vertexData;

	glm::vec3 m_currentLineColour;

};
