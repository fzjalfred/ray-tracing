#include <glm/ext.hpp>
#include "tools.hpp"

using namespace glm;

vec3 randomInUnitSphere()
{
    vec3 pos;
    do
    {
    pos = vec3(drand48(), drand48(), drand48()) * 2.0f - vec3(1.0, 1.0, 1.0);
    } while (glm::length(pos) >= 1.0);
    return pos;
}