#include <glm/glm.hpp>

using namespace glm;

struct HitRecord
{
    float m_t;
    vec3 m_position;
    vec3 m_normal;
    Material *m_material;
};