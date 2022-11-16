// Termm--Fall 2022

#include <glm/ext.hpp>
#include "Ray.hpp"
#include "HitRecord.hpp"
#include "A4.hpp"
#include <iostream>
#include <algorithm>
#include "Material.hpp"
#include "tbb/parallel_for.h"




vec3 drakBlue = vec3(0.075, 0.1, 0.4);
vec3 lightBlue = vec3(0.33, 0.42, 0.67);

float mirror_reflect_coefficient = 0.5;

using namespace std;

vec3 phongModel(vec3 fragPosition, vec3 fragNormal, const Light& ray, const vec3& eye, Material& material, const vec3& ambient) {

    // Direction from fragment to light source.
	vec3 l = normalize(fragPosition - ray.position);

    // Direction from fragment to viewer (origin - fragPosition).
    // vec3 v = normalize(ray.getOrigin()-fragPosition);
	vec3 v = normalize(- fragPosition);

	// cout<<"l: "<<l.x<<" "<<l.y<<" "<<l.z<<endl;
	// cout<<"v: "<<v.x<<" "<<v.y<<" "<<v.z<<endl;
	// cout<<dot(fragNormal, l)<<endl;

    float n_dot_l = std::max(dot(fragNormal, l), 0.0f);

	vec3 diffuse;
	diffuse = material.diffuse() * n_dot_l;

    vec3 specular = vec3(0.0);

    if (n_dot_l > 0.0) {
		// Halfway vector.
		vec3 h = normalize(v + l);
        float n_dot_h = std::max(dot(fragNormal, h), 0.0f);
		cout<<"n_dot_h: "<<dot(fragNormal, l)<<endl;
        specular = material.specular() * pow(n_dot_h, material.shininess());
    }
	// cout<<"diffuse: "<<diffuse.x<<" "<<diffuse.y<<" "<<diffuse.z<<endl;
	cout<<"specular: "<<specular.x<<" "<<specular.y<<" "<<specular.z<<endl;
    return ray.colour * (diffuse + specular);
}

vec3 tracing(Ray& ray, SceneNode* root, 
		const vec3& eye,
		// Lighting parameters  
		const glm::vec3 & ambient,
		const std::list<Light *> & lights, int reflect = 2) {
	HitRecord res;
	bool isBackground = true;
	vec3 color;
	for (auto light: lights) {
		if (root->hit(ray, 0.001f, 9999.9f, res, mat4())) {
			isBackground = false;
			// cast shadow ray to light
			Ray shadowRay = Ray(res.m_position, light->position - res.m_position);
			HitRecord shadowRecord;

			if (root->hit(shadowRay, 0.01, 9999.9f, shadowRecord, mat4())) {
				continue;
			}

			color += phongModel(res.m_position, -res.m_normal, *light, eye, *(res.m_material), ambient);
			// if (color.r<0.1&&color.g<0.1&&color.b<0.1) {
			// 	cout<<glm::to_string(color)<<endl;
			// 	cout<<glm::to_string(res.m_position)<<"pos"<<endl;
			// 	cout<<glm::to_string(res.m_normal)<<"normal"<<endl;
			// }
		}
	}

	#ifdef MirrorReflection

	if (reflect != 0) {
		vec3 reflection_direction = ray.getDirection() - 2 * res.m_normal * dot(ray.getDirection(), res.m_normal);
		Ray reflection_ray(res.m_position + 0.015*reflection_direction, reflection_direction); // with epsilon check.
		

		color = glm::mix(color, tracing(reflection_ray, root, eye, ambient, lights, reflect-1), mirror_reflect_coefficient);
		
		//cout<<glm::to_string(color)<<endl;

	}

	#endif
	
	// night blue light (0.33, 0.42, 0.67), dark (0.075, 0.1, 0.4)
	if (isBackground) {
		vec3 normal_ray = glm::normalize(ray.getDirection());
		color = (1.0 - normal_ray.x) * lightBlue/4 + normal_ray.x * drakBlue/3
		+  (1.0 - normal_ray.y)* lightBlue/4 + normal_ray.y * drakBlue/3;
	} else {
		color += res.m_material->diffuse()*ambient;
	}

	return color;
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

	uint total = h*w;
	for (uint y = 0; y < h; ++y) {
		for (uint x = 0; x < w; ++x) {
			vec3 direction = m_lowerLeftCorner + x*x_axis + (h-y-1)*y_axis;
			Ray ray = Ray(eye, direction);
			glm::vec3 color;
			color += tracing(ray, root, eye, ambient, lights);
			// Red: 
			image(x, y, 0) = (double)color.r;
			// Green: 
			image(x, y, 1) = (double)color.g;
			// Blue: 
			image(x, y, 2) = (double)color.b;

			
		}
		if (y%5 == 0) {
			uint current = y*h+1;
			cout<<"rendering... ("<<current<<", "<<total<<") ";
			printf("%0.2f%%",current/(float)total*100);
			cout<<endl;
		}
	}


	auto values = std::vector<double>(10000);
    
    tbb::parallel_for( tbb::blocked_range<int>(0,values.size()),
                       [&](tbb::blocked_range<int> r)
    {
        for (int i=r.begin(); i<r.end(); ++i)
        {
            values[i] = std::sin(i * 0.001);
        }
    });

    double t = 0.0;

    for (double value : values)
    {
        t += value;
    }

    std::cout << t << std::endl;

}
