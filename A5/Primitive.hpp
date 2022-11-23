// Termm--Fall 2022

#pragma once

#include <glm/glm.hpp>
#include "Ray.hpp"
#include "HitRecord.hpp"

class Primitive {
public:
  virtual ~Primitive();
  virtual bool hit(Ray &ray, const float& t_min, const float& t_max, HitRecord &record, const mat4& transToWorld) const = 0;
};

class Sphere : public Primitive {
public:
  virtual ~Sphere();
  bool hit(Ray &ray, const float& t_min, const float& t_max, HitRecord &record, const mat4& transToWorld) const;
};

class Cube : public Primitive {
public:
  virtual ~Cube();
  bool hit(Ray &ray, const float& t_min, const float& t_max, HitRecord &record, const mat4& transToWorld) const;
};

class NonhierSphere : public Primitive {
public:
  NonhierSphere(const glm::vec3& pos, double radius)
    : m_pos(pos), m_radius(radius)
  {
  }
  virtual ~NonhierSphere();
  bool hit(Ray &ray, const float& t_min, const float& t_max, HitRecord &record, const mat4& transToWorld) const;

private:
  glm::vec3 m_pos;
  double m_radius;
};

class NonhierBox : public Primitive {
  private:
  Primitive* cube_ptr;
public:
  NonhierBox(const glm::vec3& pos, double size);
  
  virtual ~NonhierBox();
  bool hit(Ray &ray, const float& t_min, const float& t_max, HitRecord &record, const mat4& parentTrans) const;

private:
  glm::vec3 m_pos;
  double m_size;
};
