// Termm--Fall 2022

#include <glm/ext.hpp>
#include "Ray.hpp"
#include "HitRecord.hpp"
#include "A4.hpp"

vec3 tracing(Ray& ray, SceneNode* root) {
	HitRecord res;
	vec3 color;
	if (root->hit(ray, 0.0001f, 999.9f, res)) {
		//color = res.m_material->diffuse();
	}
}


void A4_Render(
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
) {
  // Fill in raytracing code here...  

  std::cout << "F22: Calling A4_Render(\n" <<
		  "\t" << *root <<
          "\t" << "Image(width:" << image.width() << ", height:" << image.height() << ")\n"
          "\t" << "eye:  " << glm::to_string(eye) << std::endl <<
		  "\t" << "view: " << glm::to_string(view) << std::endl <<
		  "\t" << "up:   " << glm::to_string(up) << std::endl <<
		  "\t" << "fovy: " << fovy << std::endl <<
          "\t" << "ambient: " << glm::to_string(ambient) << std::endl <<
		  "\t" << "lights{" << std::endl;

	for(const Light * light : lights) {
		std::cout << "\t\t" <<  *light << std::endl;
	}
	std::cout << "\t}" << std::endl;
	std:: cout <<")" << std::endl;

	size_t h = image.height();
	size_t w = image.width();
	// camera cone info
	vec3 z_axis = -normalize(view);
	vec3 x_axis = normalize(cross(-z_axis,up));
	vec3 y_axis = normalize(up);

	float d = h/2/glm::tan(glm::radians(fovy/2));
	vec3 m_lowerLeftCorner = d*(-z_axis) - w/2*x_axis - h/2*y_axis;


	for (uint y = 0; y < h; ++y) {
		for (uint x = 0; x < w; ++x) {
			vec3 direction = m_lowerLeftCorner + x*x_axis + y*y_axis;
			Ray ray = Ray(eye, direction);
			glm::vec3 color;
			color += tracing(ray, root);
			// Red: 
			image(x, y, 0) = (double)color.r;
			// Green: 
			image(x, y, 1) = (double)color.g;
			// Blue: 
			image(x, y, 2) = (double)color.b;
		}
	}

}
