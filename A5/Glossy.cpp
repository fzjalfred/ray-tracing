// Termm--Fall 2022

#include "Glossy.hpp"
#include <glm/ext.hpp>

static vec3 randomInUnitSphere()
{
    vec3 pos;
    do
    {
    pos = vec3(drand48(), drand48(), drand48()) * 2.0f - vec3(1.0, 1.0, 1.0);
    } while (glm::length(pos) >= 1.0);
    return pos;
}

Glossy::Glossy(
	const glm::vec3& kd, const glm::vec3& ks, double shininess, double fuzz = 0.1 )
	: m_kd(kd)
	, m_ks(ks)
	, m_shininess(shininess)
    , m_fuzz(fuzz)
{}

Glossy::~Glossy()
{}


glm::vec3 Glossy::diffuse() {
	return m_kd;
}

glm::vec3 Glossy::specular() {
	return m_ks;
}

double Glossy::shininess() {
	return m_shininess;
}

double Glossy::reflectness() {
	return 0.8;
}

bool Glossy::scatter(const Ray& in, const HitRecord &rec, vec3& attenuation, Ray& scattered) {
    vec3 reflection_direction = in.getDirection() - 2 * rec.m_normal * dot(in.getDirection(), rec.m_normal);
	scattered = Ray(rec.m_position + 0.015*reflection_direction, reflection_direction + m_fuzz * randomInUnitSphere()); // with epsilon check.
	return true;
}
