// Termm-Fall 2022

#pragma once

#include "SceneNode.hpp"

class JointNode : public SceneNode {
public:
	JointNode(const std::string & name);
	virtual ~JointNode();

	void set_joint_x(double min, double init, double max);
	void set_joint_y(double min, double init, double max);

	void reset();

	struct JointRange {
		double min, init, max, range;
	};


	void rotateX(float angle);
	void rotateY(float angle);
	void rotate(double normal);
	void rotate(char axis, float angle);
	double curXIndex = 0.0;
	double curYIndex = 0.0;
	glm::mat4 rotationX;
	glm::mat4 rotationY;
	glm::mat4 selfRotate;
	JointRange m_joint_x, m_joint_y;
	float headAngle = 0.0f;
};
