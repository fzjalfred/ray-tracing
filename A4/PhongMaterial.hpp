// Termm--Fall 2022

#pragma once

#include <glm/glm.hpp>

#include "Material.hpp"

class PhongMaterial : public Material {
public:
  PhongMaterial(const glm::vec3& kd, const glm::vec3& ks, double shininess);
  virtual ~PhongMaterial();
  glm::vec3 diffuse() override;
  glm::vec3 specular() override;
  double shininess() override;

private:
  glm::vec3 m_kd;
  glm::vec3 m_ks;

  double m_shininess;
};
