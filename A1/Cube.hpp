#ifndef _CUBE_HPP_
#define _CUBE_HPP_
#include "cs488-framework/OpenGLImport.hpp"
#include "cs488-framework/GlErrorCheck.hpp"
#include "cs488-framework/ShaderProgram.hpp"


class Cube {

    // Fields related to cube geometry.
	GLuint m_cube_vao;
	GLuint m_cube_vbo;
	GLuint m_cube_ebo;

    public:
    Cube(ShaderProgram& m_shader, GLfloat x, GLfloat y, GLfloat z) {
    GLfloat vertices[] = {
    	x+0.0f, y+0.0f, z+0.0f,
     	x+1.0f, y+0.0f, z+0.0f,
     	x+1.0f, y+1.0f, z+0.0f,
		x+0.0f, y+1.0f, z+0.0f,

		x+0.0f, y+0.0f, z+1.0f,
     	x+1.0f, y+0.0f, z+1.0f,
     	x+1.0f, y+1.0f, z+1.0f,
		x+0.0f, y+1.0f, z+1.0f
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

    void draw(GLint col_uni) {
        glBindVertexArray( m_cube_vao );
		glUniform3f( col_uni, 0, 0, 1 );
		glDrawElements(GL_TRIANGLES, 3*12, GL_UNSIGNED_INT, 0);
    }
};



#endif