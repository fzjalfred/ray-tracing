// Termm--Fall 2022

#include "Primitive.hpp"

#include "Mesh.hpp"
#include <glm/glm.hpp>
#include <iostream>
using namespace glm;

Primitive::~Primitive()
{
}

Sphere::~Sphere()
{
}

bool Sphere::hit(Ray &ray, const float& t_min, const float& t_max, HitRecord &record) const {
    return NonhierSphere(vec3(0,0,0), 1.0).hit(ray, t_min, t_max, record);
}

Cube::~Cube()
{
}

bool Cube::hit(Ray &ray, const float& t_min, const float& t_max, HitRecord &record) const {
    return NonhierBox(vec3(0,0,0), 1.0).hit(ray, t_min, t_max, record);
}

NonhierSphere::~NonhierSphere()
{
}

bool NonhierSphere::hit(Ray &ray, const float& t_min, const float& t_max, HitRecord &record) const {
    vec3 s_c = ray.getOrigin() - m_pos;
    vec3 v = ray.getDirection();
    float a = dot(v,v);
    float b = 2*(dot(v,s_c));
    float c = dot(s_c, s_c)- m_radius*m_radius;

    double discriminant = b*b - 4*a*c;
    if (discriminant > 0) {
        float t_solution = (-b - sqrt(discriminant))/2*a;
        
        if (t_solution > t_min && t_solution < t_max) {
            record.m_t = t_solution;
            record.m_position = ray.getOrigin() + ray.getDirection()*t_solution;
            record.m_normal = (record.m_position - m_pos)/(float)m_radius;
            return true;
        } else {
            // std::cout<<"t_max: "<<t_max<<std::endl;
            // std::cout<<"t_solution1: "<<t_solution<<std::endl;
        }
        t_solution = (-b + sqrt(discriminant))/2*a;
        if (t_solution > t_min && t_solution < t_max) {
            record.m_t = t_solution;
            record.m_position = ray.getOrigin() + ray.getDirection()*t_solution;
            record.m_normal = (record.m_position - m_pos)/(float)m_radius;
            return true;
        } else {
            // std::cout<<"t_max: "<<t_max<<std::endl;
            // std::cout<<"t_solution2: "<<t_solution<<std::endl;
        }
        
    }
    return false;
}

NonhierBox::NonhierBox(const glm::vec3& pos, double size)
    : m_pos(pos), m_size(size)
  {
    std::vector<glm::vec3> m_vertices;
	std::vector<Triangle> m_faces;


    m_faces.push_back(Triangle(5, 6, 2));
    m_faces.push_back(Triangle(6, 7, 3));
    m_faces.push_back(Triangle(7, 8, 4));
    m_faces.push_back(Triangle(5, 1, 8));

    m_faces.push_back(Triangle(1, 2, 3));
    m_faces.push_back(Triangle(8, 7, 6));
    m_faces.push_back(Triangle(1, 5, 2));
    m_faces.push_back(Triangle(2, 6, 3));

    m_faces.push_back(Triangle(3, 7, 4));
    m_faces.push_back(Triangle(8, 1, 4));
    m_faces.push_back(Triangle(4, 1, 3));
    m_faces.push_back(Triangle(5, 8, 6));

    for (auto& face: m_faces) {
        face.v1 -= 1;
        face.v2 -= 1;
        face.v3 -= 1;
    }

    m_vertices.push_back(m_pos + vec3(0, 0, m_size));
    m_vertices.push_back(m_pos + vec3(0, 0, 0));
    m_vertices.push_back(m_pos + vec3(m_size, 0, 0));
    m_vertices.push_back(m_pos + vec3(m_size, 0, m_size));

    m_vertices.push_back(m_pos + vec3(0, m_size, m_size));
    m_vertices.push_back(m_pos + vec3(0, m_size, 0));
    m_vertices.push_back(m_pos + vec3(m_size, m_size, 0));
    m_vertices.push_back(m_pos + vec3(m_size, m_size, m_size));

    cube_ptr = new Mesh(m_vertices, m_faces);
}

NonhierBox::~NonhierBox()
{
    delete cube_ptr;
}

bool NonhierBox::hit(Ray &ray, const float& t_min, const float& t_max, HitRecord &record) const {
    return cube_ptr->hit(ray, t_min, t_max, record);
}
