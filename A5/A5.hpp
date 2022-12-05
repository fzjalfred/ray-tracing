// Termm--Fall 2022

#pragma once

#include <glm/glm.hpp>

#include "SceneNode.hpp"
#include "Light.hpp"
#include "Image.hpp"
#include <vector>


#define MirrorReflection
#define Refraction

#define TBB_MULTI_THREADING
#define SOFT_SHADOW
// #define CAMERA_DOF
#define SUPER_SAMPLING


void A5_Render(
		// What to render
		SceneNode * root,

		// Image to write to, set to a given width and height
		Image & image,

		// Viewing parameters
		const glm::vec3 & eye,
		const glm::vec3 & view,
		const glm::vec3 & up,
		double fovy,

		// Lighting parameters
		const glm::vec3 & ambient,
		const std::list<Light *> & lights
);
