// Termm--Fall 2022

#pragma once

#include <glm/glm.hpp>
#include "Ray.hpp"
#include "HitRecord.hpp"
#include "Texture.hpp"

class Material {
public:
  virtual ~Material();
  virtual glm::vec3 diffuse() = 0;
  virtual glm::vec3 specular() = 0;
  virtual double shininess() = 0;
  virtual double reflectness() = 0;
  virtual double refractness() = 0;
  virtual bool scatter(const Ray& in, const HitRecord &rec, vec3& attenuation, Ray& scattered) = 0;
  virtual bool refract(const Ray &in, const HitRecord &rec, Ray &refracted, bool flip) = 0;
  
  bool bindTexture(const std::string path); 

  Texture* m_texture = nullptr;
protected:
  Material();
};
