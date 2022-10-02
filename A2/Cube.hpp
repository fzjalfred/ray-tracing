#ifndef _CUBE_HPP_
#define _CUBE_HPP_
#include "cs488-framework/OpenGLImport.hpp"
#include "cs488-framework/GlErrorCheck.hpp"
#include "cs488-framework/ShaderProgram.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <iostream>

using namespace glm;

class Cube {

    public:
	std::vector<vec4> vertices = std::vector<vec4> (8);
	std::vector<vec4> vertices2d = std::vector<vec4> (8);

    Cube(float x, float y, float z, float length = 1.0f) {
		float cube[] = {
			x+0.0f, y+0.0f, z+0.0f,
			x+length, y+0.0f, z+0.0f,
			x+length, y+length, z+0.0f,
			x+0.0f, y+length, z+0.0f,

			x+0.0f, y+0.0f, z+length,
			x+length, y+0.0f, z+length,
			x+length, y+length, z+length,
			x+0.0f, y+length, z+length
		};
		for (int i = 0; i< 8; i+=1) {
			vertices[i] = vec4(cube[i*3], cube[i*3+1], cube[i*3+2], 1.0f);
		}

    }

	void simpleProj(mat4 view, mat4 model) {
		
		for (int i = 0; i<8; i++) {
			auto p = vertices[i];
			vec4 pos2d = view*model*p;
			vertices2d[i] = pos2d;
			// std::cout<<"x:"<< pos2d[0]<<" y:"<<pos2d[1]<<" z:"<<pos2d[2]<<std::endl;
		}
		
	}
	void draw();
};



#endif