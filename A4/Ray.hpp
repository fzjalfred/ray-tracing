#include <glm/glm.hpp>

using namespace glm;

class Ray
{
private:
    vec3 m_origin;
    vec3 m_direction;

public:
    // ctor/dtor.
    Ray() = default;
    ~Ray() = default;
    Ray(const vec3 &org, const vec3 &dir)
        :m_origin(org), m_direction(dir) {
        m_direction = normalize(m_direction);
    }

    // Getter.
    vec3 getOrigin() const { return m_origin; }
    vec3 getDirection() const { return m_direction; }

    // p(t) = origin + t*dir;
    vec3 pointAt(const float &t)const { return m_origin + m_direction * t; }
};