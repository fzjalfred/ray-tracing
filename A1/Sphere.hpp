#ifndef _SPHERE_HPP_
#define _SPHERE_HPP_
#include "cs488-framework/OpenGLImport.hpp"
#include "cs488-framework/GlErrorCheck.hpp"
#include "cs488-framework/ShaderProgram.hpp"
#include "cs488-framework/MathUtils.hpp"

#include <vector>;

extern float player_colour[3];
extern float height_scale;


class Sphere {

	const int Y_SEGMENTS = 50;
	const int X_SEGMENTS = 50;
	const float RADIUS = 0.5f;

    // Fields related to cube geometry.
	GLuint m_cube_vao;
	GLuint m_cube_vbo;
	GLuint m_cube_ebo;

	public:

	void init() {
	// 	GLfloat vertices[] = {
    // 	x+0.0f, y+0.0f, z+0.0f,
    //  	x+1.0f, y+0.0f, z+0.0f,
    //  	x+1.0f, y+1.0f, z+0.0f,
	// 	x+0.0f, y+1.0f, z+0.0f,

	// 	x+0.0f, y+0.0f, z+1.0f,
    //  	x+1.0f, y+0.0f, z+1.0f,
    //  	x+1.0f, y+1.0f, z+1.0f,
	// 	x+0.0f, y+1.0f, z+1.0f
	// };

	// GLuint indices[] = {
    // 	0, 1, 2,
	// 	0, 2, 3,

	// 	0, 1, 4,
	// 	4, 5, 1,

	// 	0, 4, 7,
	// 	0, 3, 7,

	// 	3, 7, 6,
	// 	3, 2, 6,

	// 	7, 6, 5,
	// 	7, 4, 5,

	// 	6, 2, 1,
	// 	6, 5, 1
	// };
		

        std::vector<float> sphereVertices;
        std::vector<int> sphereIndices;

        for (int y = 0; y <= Y_SEGMENTS; y++) {
            for (int x = 0; x <= X_SEGMENTS; x++) {
                float xSegment = (float)x / (float)X_SEGMENTS;
                float ySegment = (float)y / (float)Y_SEGMENTS;
                float xPos =this->x + 0.5f +
                    RADIUS*std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                float yPos = (this->y+ 0.5f + RADIUS*std::cos(ySegment * PI))/height_scale;
                float zPos = this->z+ 0.5f+
                    RADIUS*std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                sphereVertices.push_back(xPos);
                sphereVertices.push_back(yPos);
                sphereVertices.push_back(zPos);
            }
        }

        
        for (int i = 0; i < Y_SEGMENTS; i++) {
            for (int j = 0; j < X_SEGMENTS; j++) {
                sphereIndices.push_back(i * (X_SEGMENTS + 1) + j);
                sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j);
                sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j + 1);
                sphereIndices.push_back(i * (X_SEGMENTS + 1) + j);
                sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j + 1);
                sphereIndices.push_back(i * (X_SEGMENTS + 1) + j + 1);
            }
        }

        // Create the vertex array to record buffer assignments.
        glGenVertexArrays(1, &m_cube_vao);
        glBindVertexArray(m_cube_vao);

        // VBO
        glGenBuffers(1, &m_cube_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_cube_vbo);
        glBufferData(GL_ARRAY_BUFFER, sphereVertices.size()*sizeof(float), &sphereVertices[0],
                     GL_STATIC_DRAW);

        // EBO
        glGenBuffers(1, &m_cube_ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_cube_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size()*sizeof(float), &sphereIndices[0],
                     GL_STATIC_DRAW);

        // Attribute
        GLint posAttrib = m_shader.getAttribLocation("position");
        glEnableVertexAttribArray(posAttrib);
        glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        CHECK_GL_ERRORS;
        }


	GLfloat x;
	GLfloat y;
	GLfloat z;
	ShaderProgram& m_shader;

	void rePosition(int xPos, int yPos) {
		x =(float)xPos;
		y = 0.0f; 
		z = (float)yPos;
		init();
	}

	
    Sphere(ShaderProgram& m_shader, GLfloat x, GLfloat y, GLfloat z): m_shader(m_shader), x(x),y(y), z(z) {
		init();
    }

    void draw(GLint col_uni) {
        glBindVertexArray( m_cube_vao );
		glUniform3f( col_uni, player_colour[0], player_colour[1],player_colour[2]);
		glDrawElements(GL_TRIANGLES, Y_SEGMENTS*X_SEGMENTS*6, GL_UNSIGNED_INT, 0);
    }
};



#endif