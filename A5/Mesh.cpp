// Termm--Fall 2022

#include <iostream>
#include <fstream>

#include <glm/ext.hpp>

// #include "cs488-framework/ObjFileDecoder.hpp"
#include "Mesh.hpp"


// This one cost too much time.
/*
bool Mesh::triangleHit(const Ray &ray, const float &t_min, const float &t_max,
    HitRecord &ret, const vec3 &p0, const vec3 &p1,
    const vec3 &p2) const
{
	// std::cout<<"triangle hit starts."<<std::endl;
	// std::cout<<glm::to_string(p0)<<std::endl;
	// std::cout<<glm::to_string(p1)<<std::endl;
	// std::cout<<glm::to_string(p2)<<std::endl;
	vec3 normal = normalize(cross((p1-p0), (p2-p0)));
	float n_dot_dir = dot(normal, ray.getDirection());
    // no intersection.
    if (n_dot_dir == 0.0f) {
		// std::cout<<"triangle hit done."<<std::endl;
        return false;
	}
    float d = dot(-normal, p0);
	// std::cout<<"d: "<<d<<std::endl;
    float t = -(dot(normal, ray.getOrigin()) + d) / n_dot_dir;
	// std::cout<<"t: "<<t<<std::endl;
    if (t < t_min || t > t_max) {
		// std::cout<<"t_min: "<<t_min<<std::endl;
		// std::cout<<"t_max: "<<t_max<<std::endl;
        // std::cout<<"t_solution: "<<t<<std::endl;
		// std::cout<<"triangle hit done."<<std::endl;
        return false;
	}
	
	vec3 pos = ray.pointAt(t);
    ret.m_t = t;
    ret.m_position = pos;
	// if ( dot( ray.getDirection(), normal ) > 0 ) normal = -normal;
	ret.m_normal = normal;
    // judge inside or not.
    vec3 edge0 = p1 - p0;
	vec3 edge1 = p2 - p1;
	vec3 edge2 = p0 - p2;

	vec3 c0 = pos - p0;
	vec3 c1 = pos - p1;
	vec3 c2 = pos - p2;

	if (dot(normal, cross(edge0, c0)) < 0) {
		// std::cout<<"triangle hit done."<<std::endl;
		return false;
	}
	if (dot(normal, cross(edge1, c1)) < 0) {
		// std::cout<<"triangle hit done."<<std::endl;
		return false;
	}
	if (dot(normal, cross(edge2, c2)) < 0) {
		// std::cout<<"triangle hit done."<<std::endl;
		return false;
	}
	// std::cout<<"triangle hit done."<<std::endl;

    return true;
}
*/

// reference from github
bool triangleIntersection(const Ray &ray, const vec3& vertex0, const vec3& vertex1, const vec3& vertex2, float &res) {
	const float EPSILON = 0.0000001;
	glm::vec3 edge1, edge2, h, s, q;
	float a, f, u, v;
	edge1 = vertex1 - vertex0;
	edge2 = vertex2 - vertex0; 

	h = glm::cross(ray.getDirection(), edge2);
	a = glm::dot(edge1, h);
	if (a > -EPSILON && a < EPSILON)
		return false;
	f = 1 / a;
	s = ray.getOrigin() - vertex0;
	u = f * (glm::dot(s, h));
	if (u < 0.0 || u > 1.0)
		return false;
	q = glm::cross(s, edge1);
	v = f * glm::dot(ray.getDirection(), q);
	if (v < 0.0 || u + v > 1.0)
		return false;
	// At this stage we can compute t to find out where the intersection point is on the line.

	float t = f * glm::dot(edge2, q);
	if (t > EPSILON) // ray intersection
	{
		res = t;
		return true;
	}
	else // This means that there is a line intersection but not a ray intersection.
    return false;
}

// bool Mesh::triangleHit(const Ray &ray, const float &t_min, const float &t_max,
//     HitRecord &ret, const vec3 &p0, const vec3 &p1,
//     const vec3 &p2, const mat4& transToWorld, const vec3 & vn) const
// {	
// 	// vec3 normal = normalize(cross((p1-p0), (p2-p0)));
// 	vec3 normal = vn;
// 	float t;
// 	float t_unit = glm::length(transToWorld*vec4(ray.getDirection(), 0.0f));
// 	if (triangleIntersection(ray, p0, p1, p2,t)) {
// 		if (t*t_unit < t_min || t*t_unit > t_max) {
// 			return false;
// 		} else {
// 			ret.m_t = t*t_unit;
// 			if ( dot( ray.getDirection(), normal ) > 0 ) 
// 				normal = -normal;
// 			ret.m_normal = normal;
// 			//ret.m_normal = normalize(vec3(transToWorld*vec4(ret.m_normal,0.0)));
// 			ret.m_position = ray.pointAt(t);
// 			//ret.m_position = vec3(transToWorld*vec4(ret.m_position,1.0));
// 			return true;
// 		}
// 	}
// 	return false;
// }

bool Mesh::triangleHit(const Ray &ray, const float &t_min, const float &t_max,
    HitRecord &ret, const vec3 &p0, const vec3 &p1,
    const vec3 &p2, const mat4& transToWorld, const vec3 & vn, const Triangle& triangle) const
{	
	float t_unit = glm::length(transToWorld*vec4(ray.getDirection(), 0.0f));
	vec3 normal = vn;
	float n_dot_dir = dot(normal, ray.getDirection());
    // no intersection.
    if (n_dot_dir == 0.0)
        return false;
    float d = dot(-normal, p0);
    float t = -(dot(normal, ray.getOrigin()) + d) / n_dot_dir;
    if (t*t_unit < t_min || t*t_unit > t_max) {
		return false;
	}
    ret.m_t = t*t_unit;
    ret.m_position = ray.pointAt(t);
    // judge inside or not.
    vec3 r = ret.m_position - p0;
    vec3 q1 = p1 - p0;
    vec3 q2 = p2 - p0;
    float q1_len = glm::length(q1);
    float q2_len = glm::length(q2);
    float q1_dot_q2 = dot(q1, q2);
    float r_dot_q1 = dot(r, q1);
    float r_dot_q2 = dot(r, q2);
    float determinant = 1.0f / (q1_len*q1_len * q2_len*q2_len - q1_dot_q2 * q1_dot_q2);

    float omega1 = determinant * (q2_len*q2_len * r_dot_q1 - q1_dot_q2 * r_dot_q2);
    float omega2 = determinant * (-q1_dot_q2 * r_dot_q1 + q1_len*q1_len * r_dot_q2);
    if (omega1 + omega2 > 1.0f || omega1 < 0.0f || omega2 < 0.0f)
        return false;
    if ( dot( ray.getDirection(), normal ) > 0 ) 
		normal = -normal;
	ret.m_normal = normal;
	if (triangle.hasVtVn) {
		vec3 texcoord = m_texture_vertices[triangle.vt1] * (1.0f - omega1 - omega2) + m_texture_vertices[triangle.vt2] * omega1 + m_texture_vertices[triangle.vt3] * omega2;
		// std::cout<<"vt1: "<<triangle.vt1<<std::endl;
		// std::cout<<glm::to_string(m_texture_vertices[triangle.vt1])<<std::endl;
		ret.u = texcoord.x;
		ret.v = texcoord.y;
	}
    
    return true;
}


Mesh::Mesh(std::vector<glm::vec3>& m_vertices,
	std::vector<Triangle>& m_faces): m_vertices(m_vertices),
	m_faces(m_faces) {
		// float boundingMinCoor = m_vertices[0].x;
		// float boundingMaxCoor = m_vertices[0].x;
		// for (auto i: m_vertices) {
		// 	float vx = i.x;
		// 	float vy = i.y;
		// 	float vz = i.z;
		// 	float minCoor = std::min(std::min(vx, vy), vz);
		// 	float maxCoor = std::max(std::max(vx, vy), vz);
		// 	std::cout<<minCoor<<" "<<maxCoor<<std::endl;
		// 	if (minCoor < boundingMinCoor) {
		// 		boundingMinCoor = minCoor;
		// 	}
		// 	if (maxCoor > boundingMaxCoor) {
		// 		boundingMaxCoor = maxCoor;
		// 	}
		// }
		// std::cout<<m_vertices.size()<<" here cubes"<<boundingMaxCoor<<" "<<boundingMinCoor<<std::endl;
		// float mid = (boundingMaxCoor-boundingMinCoor)/2;
		// boundingVolume = new NonhierBox(vec3(mid,mid,mid), mid*2);
	}


static inline int delimit_f_index(size_t& v, size_t& vt, size_t& vn, std::string s) {
	std::string delimiter = "/";
	size_t pos = 0;
	std::string token;
	int ret = 0;
	if ((pos = s.find(delimiter)) != std::string::npos) {
		token = s.substr(0, pos);
		v = std::stoul(token);
		ret |= 1 << 0;
		s.erase(0, pos + delimiter.length());
		if ((pos = s.find(delimiter)) != std::string::npos) {
			token = s.substr(0, pos);
			vt = std::stoul(token);
			ret |= 1 << 1;
			s.erase(0, pos + delimiter.length());
			if (s.size()>0) {
				token = s;
				vn = std::stoul(token);
				ret |= 1 << 2;
			}
		}
	} else {
		v = std::stoul(s);
		ret |= 1 << 0;
	}
	return ret;
}


Mesh::Mesh( const std::string& fname )
	: m_vertices()
	, m_faces()
{
	std::string code;
	// vertice buffer
	double vx, vy, vz;
	// f buffer
	size_t v1, v2, v3;
	size_t vt1, vt2, vt3;
	size_t vn1, vn2, vn3;
	// f string buffer
	std::string st1, st2, st3;

	std::ifstream ifs( fname.c_str() );
	while( ifs >> code ) {
		if( code == "v" ) {
			ifs >> vx >> vy >> vz;
			float minCoor = std::min(std::min(vx, vy), vz);
			float maxCoor = std::max(std::max(vx, vy), vz);
			if (minCoor < boundingMinCoor) {
				boundingMinCoor = minCoor;
			}
			if (maxCoor > boundingMaxCoor) {
				boundingMaxCoor = maxCoor;
			}
			m_vertices.push_back( glm::vec3( vx, vy, vz ) );
		} else if( code == "f" ) {
			ifs >> st1 >> st2 >> st3;
			
			int status = delimit_f_index(v1, vt1, vn1, st1);
			delimit_f_index(v2, vt2, vn2, st2);
			delimit_f_index(v3, vt3, vn3, st3);
			
			if ((status>>1&1) && (status>>2&1)) {
				m_faces.push_back( Triangle( v1 - 1, v2 - 1, v3 - 1, vt1 - 1, vt2 - 1, vt3 - 1, vn1 - 1 ) );
			} else {
				m_faces.push_back( Triangle( v1 - 1, v2 - 1, v3 - 1 ) );
			}
			
		} else if ( code == "vt") {
			ifs >> vx >> vy;
			m_texture_vertices.push_back( glm::vec3( vx, vy, 0.0 ) );
		} else if ( code == "vn") {
			ifs >> vx >> vy >> vz;
			m_normals.push_back( glm::vec3( vx, vy, vz ) );
		}
	}
	if (m_vertices.size()>1) {
		std::cout<<fname<<std::endl;
	}
	if (m_faces.size() > 12) {
		boundingVolume = new NonhierBox(vec3(boundingMinCoor,boundingMinCoor,boundingMinCoor), boundingMaxCoor-boundingMinCoor);
	}
	
}

std::ostream& operator<<(std::ostream& out, const Mesh& mesh)
{
  out << "mesh {";
  
  
//   for( size_t idx = 0; idx < mesh.m_vertices.size(); ++idx ) {
//   	const vec3& v = mesh.m_vertices[idx];
//   	out << glm::to_string( v );
//   }


  out << "}";
  return out;
}

static bool insideBound(vec3 pos, float minBound, float maxBound) {
	return (minBound<=pos.x && pos.x<=maxBound) && (minBound<=pos.y && pos.y<=maxBound) && (minBound<=pos.z && pos.z<=maxBound);
}


bool Mesh::hit(Ray &ray, const float& t_min, const float& t_max, HitRecord &record, const mat4& transToWorld) const {
	bool hitAny = false;
	float closest = t_max;
	HitRecord tmpHit;

#ifdef RENDER_BOUNDING_VOLUMES

	if (boundingVolume != nullptr && boundingVolume->hit(ray, t_min, t_max, tmpHit, transToWorld)) {
		closest = tmpHit.m_t;
		hitAny = true;
		record = tmpHit;
	}
	if (boundingVolume == nullptr || insideBound(ray.getOrigin(), boundingMinCoor, boundingMaxCoor)) {
		for (auto triangle : m_faces) {
			vec3 normal;
			if (triangle.hasVtVn) {
				normal = m_normals[triangle.vn];
			} else {
				normal = normalize(cross((m_vertices[triangle.v2]-m_vertices[triangle.v1]), (m_vertices[triangle.v3]-m_vertices[triangle.v1])));
			}
			if (triangleHit(ray, t_min, closest, tmpHit, m_vertices[triangle.v1], m_vertices[triangle.v2], m_vertices[triangle.v3], transToWorld, normal)) {
				closest = tmpHit.m_t;
				hitAny = true;
				record = tmpHit;
			}
		}
	}
#else

	if (boundingVolume == nullptr 
	|| boundingVolume->hit(ray, t_min, t_max, tmpHit, transToWorld) || 
	insideBound(ray.getOrigin(), boundingMinCoor, boundingMaxCoor)
	) {
		for (const auto& triangle : m_faces) {
			vec3 normal;
			if (triangle.hasVtVn) {
				normal = m_normals[triangle.vn];
			} else {
				normal = normalize(cross((m_vertices[triangle.v2]-m_vertices[triangle.v1]), (m_vertices[triangle.v3]-m_vertices[triangle.v1])));
			}

			if (triangleHit(ray, t_min, closest, tmpHit, m_vertices[triangle.v1], m_vertices[triangle.v2], m_vertices[triangle.v3], transToWorld, normal, triangle)) {
				closest = tmpHit.m_t;
				hitAny = true;
				record = tmpHit;
			}
		}
	}

#endif
	
	return hitAny;
}