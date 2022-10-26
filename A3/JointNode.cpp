// Termm-Fall 2022

#include "JointNode.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "cs488-framework/MathUtils.hpp"

#include <iostream>
using namespace std;
//---------------------------------------------------------------------------------------
JointNode::JointNode(const std::string& name)
	: SceneNode(name)
{
	m_nodeType = NodeType::JointNode;
}

//---------------------------------------------------------------------------------------
JointNode::~JointNode() {

}
 //---------------------------------------------------------------------------------------
void JointNode::set_joint_x(double min, double init, double max) {
	m_joint_x.min = min;
	m_joint_x.init = init;
	m_joint_x.max = max;
	m_joint_x.range = max - min;
	//curXangle = 0;
	rotateX(m_joint_x.init);
	curXIndex = (init-min)/m_joint_x.range;
}

//---------------------------------------------------------------------------------------
void JointNode::set_joint_y(double min, double init, double max) {
	m_joint_y.min = min;
	m_joint_y.init = init;
	m_joint_y.max = max;
	m_joint_y.range = max - min;
	//curYangle = 0;
	rotateY(m_joint_y.init);
	curYIndex = (init-min)/m_joint_y.range;
}

void JointNode::reset() {
	curXIndex = (m_joint_x.init-m_joint_x.min)/m_joint_x.range;
	curYIndex = (m_joint_y.init-m_joint_y.min)/m_joint_y.range;
	jointRotate = glm::mat4();
	rotationX = glm::mat4();
	rotationY = glm::mat4();
	selfRotate = glm::mat4();
	headAngle = 0.0f;
}


void JointNode::rotateX(float angle) {
	// cout<<"current x angle: "<<curXangle<<endl;
	// cout<<"rotate x angle: "<<angle<<endl;
	rotationX = glm::rotate(degreesToRadians(angle), glm::vec3(1,0,0));
	jointRotate = rotationX*rotationY*selfRotate;
	//curXangle = angle;
}

void JointNode::rotateY(float angle) {
	// cout<<"current y angle: "<<curYangle<<endl;
	rotationY = glm::rotate(degreesToRadians(angle), glm::vec3(0,1,0));
	jointRotate = rotationX*rotationY*selfRotate;
	// curYangle = angle;
}

void JointNode::rotate(double normal) {


	if (!std::isnan(curXIndex)) {
		curXIndex += normal;
		if (curXIndex > 1) {
			curXIndex = 1;
		}
		if (curXIndex < 0) {
			curXIndex = 0;
		}
		rotateX(m_joint_x.min+m_joint_x.range*curXIndex);
	}
	
	
	if (!std::isnan(curYIndex)) {
		curYIndex += normal;
		if (curYIndex > 1) {
			curYIndex = 1;
		}
		if (curYIndex < 0) {
			curYIndex = 0;
		}
		rotateY(m_joint_y.min+m_joint_y.range*curYIndex);
	}
	
	
}

void JointNode::rotate(char axis, float angle) {
	glm::vec3 rot_axis;

	switch (axis) {
		case 'x':
			rot_axis = glm::vec3(1,0,0);
			break;
		case 'y':
			rot_axis = glm::vec3(0,1,0);
	        break;
		case 'z':
			rot_axis = glm::vec3(0,0,1);
	        break;
		default:
			break;
	}
	if (angle<60 && angle>-60) {
		headAngle = angle;
		glm::mat4 rot_matrix = glm::rotate(degreesToRadians(angle), rot_axis);
		selfRotate = rot_matrix;
		jointRotate = rotationX*rotationY*selfRotate;
	}
}