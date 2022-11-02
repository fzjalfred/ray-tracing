// Termm--Fall 2022

#include "Primitive.hpp"

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

}

Cube::~Cube()
{
}

bool Cube::hit(Ray &ray, const float& t_min, const float& t_max, HitRecord &record) const {

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
            std::cout<<"t_max: "<<t_max<<std::endl;
            std::cout<<"t_solution1: "<<t_solution<<std::endl;
        }
        t_solution = (-b + sqrt(discriminant))/2*a;
        if (t_solution > t_min && t_solution < t_max) {
            record.m_t = t_solution;
            record.m_position = ray.getOrigin() + ray.getDirection()*t_solution;
            record.m_normal = (record.m_position - m_pos)/(float)m_radius;
            return true;
        } else {
            std::cout<<"t_max: "<<t_max<<std::endl;
            std::cout<<"t_solution2: "<<t_solution<<std::endl;
        }
        
    }
    return false;
}

NonhierBox::~NonhierBox()
{
}

bool NonhierBox::hit(Ray &ray, const float& t_min, const float& t_max, HitRecord &record) const {

}
