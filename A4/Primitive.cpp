// Termm--Fall 2022

#include "Primitive.hpp"

#include <glm/glm.hpp>

using namespace glm;

Primitive::~Primitive()
{
}

Sphere::~Sphere()
{
}

Cube::~Cube()
{
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
        
    }
}

NonhierBox::~NonhierBox()
{
}
