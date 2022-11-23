// Termm--Fall 2022

#pragma once

#include "SceneNode.hpp"
#include "Primitive.hpp"
#include "Material.hpp"

class GeometryNode : public SceneNode {
public:
	GeometryNode( const std::string & name, Primitive *prim, 
		Material *mat = nullptr );

	void setMaterial( Material *material );

	Material *m_material;
	Primitive *m_primitive;

	virtual bool hit(Ray &ray, const float& t_min, const float& t_max, HitRecord &record, const mat4& parentTrans);
};
