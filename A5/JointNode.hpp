// Termm--Fall 2022

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

	virtual bool hit(Ray &ray, const float& t_min, const float& t_max, HitRecord &record, const mat4& parentTrans);
	JointRange m_joint_x, m_joint_y;
};
