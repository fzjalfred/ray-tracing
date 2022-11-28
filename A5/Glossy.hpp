// Termm--Fall 2022

#pragma once

#include <glm/glm.hpp>

#include "Material.hpp"
#include "Ray.hpp"
#include "HitRecord.hpp"

class Glossy : public Material {
public:
  Glossy(const glm::vec3& kd, const glm::vec3& ks, double shininess, double fuzz);
  virtual ~Glossy();
  glm::vec3 diffuse() override;
  glm::vec3 specular() override;
  double shininess() override;
  double reflectness() override;
  double refractness() override;
  bool scatter(const Ray& in, const HitRecord &rec, vec3& attenuation, Ray& scattered) override;
  bool refract(const Ray &in, const HitRecord &rec, Ray &refracted, bool flip) override;

private:
  glm::vec3 m_kd;
  glm::vec3 m_ks;

  double m_shininess;
  double m_fuzz;
};