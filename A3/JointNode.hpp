// Termm-Fall 2022

#pragma once

#include "SceneNode.hpp"

class JointNode : public SceneNode {
public:
	JointNode(const std::string & name);
	virtual ~JointNode();

	void set_joint_x(double min, double init, double max);
	void set_joint_y(double min, double init, double max);

	struct JointRange {
		double min, init, max;
	};


	void rotate(char axis, float angle);
	float cur_angleX = 0.0f;
	float cur_angleY = 0.0f;
	JointRange m_joint_x, m_joint_y;
};
