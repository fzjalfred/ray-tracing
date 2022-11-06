// Termm--Fall 2022

#include "GeometryNode.hpp"

//---------------------------------------------------------------------------------------
GeometryNode::GeometryNode(
	const std::string & name, Primitive *prim, Material *mat )
	: SceneNode( name )
	, m_material( mat )
	, m_primitive( prim )
{
	m_nodeType = NodeType::GeometryNode;
}

void GeometryNode::setMaterial( Material *mat )
{
	// Obviously, there's a potential memory leak here.  A good solution
	// would be to use some kind of reference counting, as in the 
	// C++ shared_ptr.  But I'm going to punt on that problem here.
	// Why?  Two reasons:
	// (a) In practice we expect the scene to be constructed exactly
	//     once.  There's no reason to believe that materials will be
	//     repeatedly overwritten in a GeometryNode.
	// (b) A ray tracer is a program in which you compute once, and 
	//     throw away all your data.  A memory leak won't build up and
	//     crash the program.

	m_material = mat;
}

bool GeometryNode::hit(Ray &ray, const float& t_min, const float& t_max, HitRecord &record, const mat4& parentTrans) {
	Ray inverse_transform_ray = Ray(vec3(invtrans*vec4(ray.getOrigin(), 1.0)), vec3(invtrans*vec4(ray.getDirection(), 0.0)));
	bool hitAny = m_primitive->hit(inverse_transform_ray, t_min, t_max, record, parentTrans*trans);
	// record.m_position = vec3(trans*vec4(record.m_position,1.0));
	// record.m_normal = normalize(vec3(trans*vec4(record.m_normal, 0.0)));
	record.m_material = m_material;
	return hitAny;
}