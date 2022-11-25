// Termm--Fall 2022

#include "PhongMaterial.hpp"
#include <glm/ext.hpp>

PhongMaterial::PhongMaterial(
	const glm::vec3& kd, const glm::vec3& ks, double shininess )
	: m_kd(kd)
	, m_ks(ks)
	, m_shininess(shininess)
{}

PhongMaterial::~PhongMaterial()
{}


glm::vec3 PhongMaterial::diffuse() {
	return m_kd;
}

glm::vec3 PhongMaterial::specular() {
	return m_ks;
}

double PhongMaterial::shininess() {
	return m_shininess;
}

bool PhongMaterial::scatter(const Ray& in, const HitRecord &rec, vec3& attenuation, Ray& scattered) {
	vec3 reflection_direction = in.getDirection() - 2 * rec.m_normal * dot(in.getDirection(), rec.m_normal);
	scattered = Ray(rec.m_position + 0.015*reflection_direction, reflection_direction); // with epsilon check.
	return true;
}
