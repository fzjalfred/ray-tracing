#pragma once

#include "Material.hpp"
#include <glm/glm.hpp>
#include <iostream>

using namespace glm;

struct HitRecord
{
    float m_t;
    vec3 m_position;
    vec3 m_normal;
    Material *m_material;
    
};

std::ostream & operator << (std::ostream & os, const HitRecord & record);