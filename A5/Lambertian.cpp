// Termm--Fall 2022

#include "Lambertian.hpp"
#include <glm/ext.hpp>
#include "tools.hpp"
// static vec3 randomInUnitSphere()
// {
//     vec3 pos;
//     do
//     {
//     pos = vec3(drand48(), drand48(), drand48()) * 2.0f - vec3(1.0, 1.0, 1.0);
//     } while (glm::length(pos) >= 1.0);
//     return pos;
// }

Lambertian::Lambertian(
	const glm::vec3& kd, const glm::vec3& ks, double shininess )
	: m_kd(kd)
	, m_ks(ks)
	, m_shininess(shininess)
{}

Lambertian::~Lambertian()
{}


glm::vec3 Lambertian::diffuse() {
	return m_kd;
}

glm::vec3 Lambertian::specular() {
	return m_ks;
}

double Lambertian::shininess() {
	return m_shininess;
}

double Lambertian::reflectness() {
	return 0.8;
}

bool Lambertian::scatter(const Ray& in, const HitRecord &rec, vec3& attenuation, Ray& scattered) {
    vec3 target = rec.m_position + rec.m_normal + randomInUnitSphere();
    vec3 direction = normalize(target - rec.m_position);
    scattered = Ray(rec.m_position + 0.015*direction, direction);
	return true;
}

bool Lambertian::refract(const Ray &in, const HitRecord &rec, Ray &refracted, bool flip = false) {
	return false;
}

double Lambertian::refractness() {
	return 0.0;
};