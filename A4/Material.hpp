// Termm--Fall 2022

#pragma once

#include <glm/glm.hpp>

class Material {
public:
  virtual ~Material();
  virtual glm::vec3 diffuse() = 0;
  virtual glm::vec3 specular() = 0;

protected:
  Material();
};
