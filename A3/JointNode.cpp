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
	rotateX(m_joint_x.init);
}

//---------------------------------------------------------------------------------------
void JointNode::set_joint_y(double min, double init, double max) {
	m_joint_y.min = min;
	m_joint_y.init = init;
	m_joint_y.max = max;
	rotateX(m_joint_y.init);
}


void JointNode::rotateX(float angle) {
	glm::mat4 rotation = glm::rotate(degreesToRadians(angle), glm::vec3(1,0,0));
	trans = rotation * trans;
}

void JointNode::rotateY(float angle) {
	glm::mat4 rotation = glm::rotate(degreesToRadians(angle), glm::vec3(0,1,0));
	trans = rotation * trans;
}