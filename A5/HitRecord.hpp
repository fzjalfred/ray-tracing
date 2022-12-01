#pragma once

#include <glm/glm.hpp>
#include <iostream>

using namespace glm;

class Material;

struct HitRecord
{
    float m_t;
    vec3 m_position;
    vec3 m_normal;
    Material *m_material;

    double u;
    double v;
    
};

std::ostream & operator << (std::ostream & os, const HitRecord & record);