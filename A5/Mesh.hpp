// Termm--Fall 2022

#pragma once

#include <vector>
#include <iosfwd>
#include <string>

#include <glm/glm.hpp>

#include "Primitive.hpp"

// Use this #define to selectively compile your code to render the
// bounding boxes around your mesh objects. Uncomment this option
// to turn it on.
// #define RENDER_BOUNDING_VOLUMES

struct Triangle
{
	size_t v1;
	size_t v2;
	size_t v3;

	bool hasVtVn = false;
	size_t vt1;
	size_t vt2;
	size_t vt3;

	size_t vn;

	Triangle( size_t pv1, size_t pv2, size_t pv3 )
		: v1( pv1 )
		, v2( pv2 )
		, v3( pv3 )
	{}

	Triangle( size_t pv1, size_t pv2, size_t pv3, size_t vt1, size_t vt2, size_t vt3, size_t vn)
		: v1( pv1 )
		, v2( pv2 )
		, v3( pv3 )
		, vt1( vt1 )
		, vt2( vt2 )
		, vt3( vt3 )
		, vn(vn)
	{
		hasVtVn = true;
	}
};

// A polygonal mesh.
class Mesh : public Primitive {
public:
  Mesh( const std::string& fname );
  Mesh(std::vector<glm::vec3>& m_vertices,
	std::vector<Triangle>& m_faces);
  bool hit(Ray &ray, const float& t_min, const float& t_max, HitRecord &record, const mat4& transToWorld) const;

  float boundingMinCoor = std::numeric_limits<float>::max();
  float boundingMaxCoor = std::numeric_limits<float>::min();
  
private:
	std::vector<glm::vec3> m_vertices;
	std::vector<Triangle> m_faces;
	std::vector<glm::vec3> m_texture_vertices;
	std::vector<glm::vec3> m_normals;

	Primitive* boundingVolume = nullptr;

    friend std::ostream& operator<<(std::ostream& out, const Mesh& mesh);

	bool triangleHit(const Ray &ray, const float &t_min, const float &t_max,
    HitRecord &ret, const vec3 &p0, const vec3 &p1,
    const vec3 &p2, const mat4& transToWorld, const vec3 & vn, const Triangle& triangle) const;
};
