// Termm--Fall 2022

#include "PhongMaterial.hpp"
#include <glm/ext.hpp>

PhongMaterial::PhongMaterial(
	const glm::vec3& kd, const glm::vec3& ks, double shininess, double reflect_coefficient, double refractness )
	: m_kd(kd)
	, m_ks(ks)
	, m_shininess(shininess)
	, m_reflect(reflect_coefficient)
	, m_refractness(refractness)
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

double PhongMaterial::reflectness() {
	return m_reflect;
}

bool PhongMaterial::scatter(const Ray& in, const HitRecord &rec, vec3& attenuation, Ray& scattered) {
	vec3 reflection_direction = in.getDirection() - 2 * rec.m_normal * dot(in.getDirection(), rec.m_normal);
	scattered = Ray(rec.m_position + 0.015*reflection_direction, reflection_direction); // with epsilon check.
	return true;
}

bool PhongMaterial::refract(const Ray &in, const HitRecord &rec,
    Ray &refracted, bool flip = false) {
	float refIdx = 1.01f;
	float niOvernt;
	float reflect_prob;
	float cosine;
	
	Ray reflected;
	vec3 normal;

	vec3 att = vec3();
	scatter(in, rec, att, reflected);
	// from inside to outside
	if (flip) {
		niOvernt = refIdx;
		normal = -rec.m_normal;
		cosine = refIdx * dot(in.getDirection(), rec.m_normal) / glm::length(in.getDirection());
	} else {
		niOvernt = 1.0/refIdx;
		normal = rec.m_normal;
		cosine = -dot(in.getDirection(), rec.m_normal) / glm::length(in.getDirection());
	}
	
    vec3 uv = in.getDirection();
    float dt = dot(uv, normal);
    float discriminant = 1.0f - niOvernt * niOvernt * (1.0f - dt * dt);
    if (discriminant > 0.0f)
    {
        refracted = Ray(rec.m_position, (uv - normal * dt) * niOvernt - normal * sqrt(discriminant));
        reflect_prob = schlick(cosine, 1/niOvernt);
    }
    else {
		reflect_prob = 1.0f;
		refracted = reflected;
	}
	// if (drand48() < reflect_prob) {
	// 	refracted = reflected;
	// }
	return true;
}

double PhongMaterial::refractness() {
	return m_refractness;
};


float PhongMaterial::schlick(float cosine, float ref_idx) const
{
    float r0 = (1.0f - ref_idx) / (1.0f + ref_idx);
    r0 = r0 * r0;
    return r0 + (1.0f - r0) * pow((1.0f - cosine), 5.0f);
}