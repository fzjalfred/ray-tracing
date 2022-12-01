// Termm--Fall 2022

#include <glm/ext.hpp>
#include "Ray.hpp"
#include "HitRecord.hpp"
#include "A5.hpp"
#include <iostream>
#include <algorithm>
#include "Material.hpp"
#include <tbb/parallel_for.h>
#include <tbb/global_control.h>
#include <chrono>
#include <pthread.h>
#include "tools.hpp"
using namespace std::chrono;


vec3 drakBlue = vec3(0.075, 0.1, 0.4);
vec3 lightBlue = vec3(0.33, 0.42, 0.67);


double camera_dof = 1;
int super_samples = 10;
float mirror_reflect_coefficient = 0.9;
int NUM_THREADS = 4;

int soft_shadow_ray_samples = 10;

std::vector<std::vector<double>> shadow_map;
using namespace std;

float mdistance = 0;

vec3 phongModel(vec3 fragPosition, vec3 fragNormal, const Light& ray, const vec3& eye, HitRecord& res, const vec3& ambient) {


	Material& material = *(res.m_material);
    // Direction from fragment to light source.
	vec3 l = normalize(fragPosition - ray.position);

    // Direction from fragment to viewer (origin - fragPosition).
    // vec3 v = normalize(ray.getOrigin()-fragPosition);
	vec3 v = normalize(eye - fragPosition);


	// cout<<"l: "<<l.x<<" "<<l.y<<" "<<l.z<<endl;
	// cout<<"v: "<<v.x<<" "<<v.y<<" "<<v.z<<endl;
	// cout<<dot(fragNormal, l)<<endl;

    float n_dot_l = std::max(dot(fragNormal, l), 0.0f);

	vec3 R = 2*(dot(fragNormal, -l))*fragNormal+l;

	vec3 diffuse;
	if (res.m_material->m_texture != nullptr) {
		vec3 sampled = res.m_material->m_texture->sample(res.u, res.v, vec3());
		diffuse = sampled * n_dot_l;
	} else
	diffuse = material.diffuse() * n_dot_l;

    vec3 specular = vec3(0.0);

    if (n_dot_l > 0.0) {
		// Halfway vector.
		vec3 h = normalize(v + l);
        float n_dot_h = std::max(dot(R, v), 0.0f);

        specular = material.specular() * pow(n_dot_h, material.shininess());
    }
	// cout<<"diffuse: "<<diffuse.x<<" "<<diffuse.y<<" "<<diffuse.z<<endl;
	// cout<<"specular: "<<specular.x<<" "<<specular.y<<" "<<specular.z<<endl;
    return ray.colour * (diffuse + specular);
}

int counterRef = 0;

vec3 tracing(Ray& ray, SceneNode* root, 
		const vec3& eye,
		// Lighting parameters  
		const glm::vec3 & ambient,
		const std::list<Light *> & lights, int reflect = 2, bool exitingRefract = false) {
	HitRecord res;
	bool isBackground = true;
	vec3 color;
	for (auto light: lights) {
		if (root->hit(ray, 0.001f, 9999.9f, res, mat4())) {
			isBackground = false;

			double soft_shadow_coefficient = 1.0;
#ifndef SOFT_SHADOW
			// cast shadow ray to light
			Ray shadowRay = Ray(res.m_position, light->position - res.m_position);
			HitRecord shadowRecord;

			if (root->hit(shadowRay, 0.01, 9999.9f, shadowRecord, mat4())) {
				continue;
			}
#else
			// cast shadow ray to light
			int hits_count = 0;
			Ray directShadowRay = Ray(res.m_position, light->position - res.m_position);
			HitRecord directShadowRecord;
			double sample_size;
			
			if (root->hit(directShadowRay, 0.01, 9999.9f, directShadowRecord, mat4())) {
				double hit2LightDistance = glm::length(light->position - res.m_position);
				double Occulusion2LightDistance = glm::length(light->position - directShadowRecord.m_position);
				hits_count++;
				sample_size = (1 - Occulusion2LightDistance/hit2LightDistance)*30.0;
			}
			for (int i = 0; i<=soft_shadow_ray_samples; i++) {
				Ray shadowRay = Ray(res.m_position, light->position - res.m_position + sample_size*randomInUnitSphere());
				HitRecord shadowRecord;

				if (root->hit(shadowRay, 0.01, 9999.9f, shadowRecord, mat4())) {
					hits_count++;
				}
			}
			
			soft_shadow_coefficient = 1 - hits_count/soft_shadow_ray_samples;
#endif
			color += soft_shadow_coefficient*phongModel(res.m_position, -res.m_normal, *light, eye, res, ambient);
			// if (color.r<0.1&&color.g<0.1&&color.b<0.1) {
			// 	cout<<glm::to_string(color)<<endl;
			// 	cout<<glm::to_string(res.m_position)<<"pos"<<endl;
			// 	cout<<glm::to_string(res.m_normal)<<"normal"<<endl;
			// }
		}
	}

	#ifdef MirrorReflection

	if (reflect != 0 && isBackground == false && res.m_material->reflectness() != 0.0) {
		// vec3 reflection_direction = ray.getDirection() - 2 * res.m_normal * dot(ray.getDirection(), res.m_normal);
		Ray scattered; // with epsilon check.
		vec3 attenuation; 
		if (res.m_material != nullptr) {
			res.m_material->scatter(ray, res, attenuation, scattered);
			color = glm::mix(color, tracing(scattered, root, eye, ambient, lights, reflect-1), res.m_material->reflectness());
		}

	}

	#endif

	#ifdef Refraction

	if (reflect != 0 && isBackground == false && res.m_material->refractness() != 0.0) {
		// vec3 reflection_direction = ray.getDirection() - 2 * res.m_normal * dot(ray.getDirection(), res.m_normal);
		Ray scattered; // with epsilon check.
		vec3 attenuation; 
		if (res.m_material != nullptr) {
			res.m_material->refract(ray, res, scattered, exitingRefract);
			color = glm::mix(color, tracing(scattered, root, eye, ambient, lights, reflect-1, !exitingRefract), res.m_material->refractness());
		}

	}

	#endif
	
	// night blue light (0.33, 0.42, 0.67), dark (0.075, 0.1, 0.4)
	if (isBackground) {
		vec3 normal_ray = glm::normalize(ray.getDirection());
		color = 2*((1.0 - normal_ray.x) * lightBlue/4 + normal_ray.x * drakBlue/3
		+  (1.0 - normal_ray.y)* lightBlue/4 + normal_ray.y * drakBlue/3);
	} else {
		color += res.m_material->diffuse()*ambient;
	}

	return color;
}

struct thread_data {
    int m;
    int n;

    std::vector<pair<uint, uint>>& tasks;
    vec3 m_lowerLeftCorner;
    vec3 x_axis;
    vec3 y_axis;
    size_t h;
    Image& image;

    SceneNode* root;
    const glm::vec3& eye;
    const glm::vec3& ambient;
    const std::list<Light*>& lights;

	uint& counter;
	uint total;
};

void* f_task(void* threadarg) {
    struct thread_data* my_data;
    my_data = (struct thread_data*)threadarg;

	int m = my_data->m;
    int n = my_data->n;

    std::vector<pair<uint, uint>>& tasks = my_data->tasks;
    vec3 m_lowerLeftCorner = my_data->m_lowerLeftCorner;
    vec3 x_axis = my_data->x_axis;
    vec3 y_axis = my_data->y_axis;
    size_t h = my_data->h;
    Image& image = my_data->image;
    uint& counter = my_data->counter;
	uint total = my_data->total;

    SceneNode* root = my_data->root;
    const glm::vec3& eye = my_data->eye;
    const glm::vec3& ambient = my_data->ambient;
    const std::list<Light*>& lights = my_data->lights;

    for (int i = m; i <= n; ++i) {
        uint x = tasks[i].first;
        uint y = tasks[i].second;
        glm::vec3 color;

	glm::vec3 camera_eye = eye;


#ifndef SUPER_SAMPLING
        vec3 direction = m_lowerLeftCorner + x * x_axis + (h - y - 1) * y_axis;
        Ray ray = Ray(camera_eye, direction);
        color += tracing(ray, root, eye, ambient, lights);
#else
        for (int sps = 0; sps < super_samples; ++sps) {
            float u = static_cast<float>(x + drand48());
            float v = static_cast<float>(y + drand48());
            vec3 direction =
                m_lowerLeftCorner + u * x_axis + (h - v - 1) * y_axis;
#ifdef CAMERA_DOF
			vec3 move = vec3(drand48()*0.12, drand48()*0.12, 0);
			float focal_plane = 10.0f;
			vec3 focal_direction = direction* (mdistance-focal_plane) / mdistance;
			camera_eye = eye + move;
			Ray ray = Ray(camera_eye, focal_direction-move);
#else
			Ray ray = Ray(camera_eye, direction);
#endif
            
            color += tracing(ray, root, eye, ambient, lights);
        }
        color /= static_cast<float>(super_samples);
#endif
        // Red:
        image(x, y, 0) = (double)color.r;
        // Green:
        image(x, y, 1) = (double)color.g;
        // Blue:
        image(x, y, 2) = (double)color.b;

        counter++;
        if (counter % 10000 == 0) {
            cout << "rendering... (" << counter << ", " << total << ") ";
            printf("%0.2f%%", counter / (float)total * 100);
            cout << endl;
        }

    }

    pthread_exit(NULL);
};

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
) {
  // Fill in raytracing code here...  

  std::cout << "F22: Calling A5_Render(\n" <<
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

	// camera_dof = glm::length(eye)*camera_dof;

	mdistance = h/2/glm::tan(glm::radians(fovy/2)); // 
	vec3 m_lowerLeftCorner = mdistance*(-z_axis) - w/2*x_axis - h/2*y_axis;

	uint total = h*w;
	auto tasks = std::vector<pair<uint,uint>>();
	shadow_map = std::vector<std::vector<double>> (w+1, std::vector<double>(h+1));
	for (uint y = 0; y < h; ++y) {
		for (uint x = 0; x < w; ++x) {
			tasks.emplace_back(x, y);
			shadow_map[x][y] = 0.0;
		}
	}
	auto start = high_resolution_clock::now();

#ifndef TBB_MULTI_THREADING
	for (uint t = 0; t<h*w; t++) {
		uint x = tasks[t].first;
		uint y = tasks[t].second;
		glm::vec3 color;
		#ifndef SUPER_SAMPLING
		vec3 direction = m_lowerLeftCorner + x*x_axis + (h-y-1)*y_axis;
		Ray ray = Ray(eye, direction);
		color += tracing(ray, root, eye, ambient, lights);
		#else
		for (int sps = 0; sps < super_samples; ++sps)
        {
            float u = static_cast<float>(x + drand48());
            float v = static_cast<float>(y + drand48());
			vec3 direction = m_lowerLeftCorner + u*x_axis + (h-v-1)*y_axis;
            Ray ray = Ray(eye, direction);
            color += tracing(ray, root, eye, ambient, lights);
        }
		color /= static_cast<float>(super_samples);
		#endif
		
		// Red: 
		image(x, y, 0) = (double)color.r;
		// Green: 
		image(x, y, 1) = (double)color.g;
		// Blue: 
		image(x, y, 2) = (double)color.b;
		if (t%10000 == 0) {
			cout<<"rendering... ("<<t<<", "<<total<<") ";
			printf("%0.2f%%",t/(float)total*100);
			cout<<endl;
		}
	}
#else
	uint counter = 0;
	pthread_t threads[NUM_THREADS];
	struct thread_data* td[NUM_THREADS];
	pthread_mutex_t count_mutex;

	int rc;
	for(int i = 0; i < NUM_THREADS; i++ ) {
		int m = (i)*total/NUM_THREADS;
		int n = (i+1)*total/NUM_THREADS - 1;
		td[i] = new thread_data{m,n,tasks,m_lowerLeftCorner,x_axis,y_axis, h, image, root, eye, ambient, lights, counter, total};
      	cout << "main() : creating thread, ranged from " << m << " " << n << endl;
      	rc = pthread_create(&threads[i], NULL, f_task, (void *)td[i]);
      
      	if (rc) {
        	 cout << "Error:unable to create thread," << rc << endl;
        	 exit(-1);
      	}
   	}
	for (int i = 0; i < NUM_THREADS; i++) {
		pthread_join( threads[i], NULL);
	}
   	

#endif
	
    auto stop = high_resolution_clock::now();
	auto duration = duration_cast<seconds>(stop - start);
	std::cout<<"Time consumption: " << duration.count() <<" sec."<< std::endl;
	
	
}


// tbb::global_control c(tbb::global_control::max_allowed_parallelism, 4);
	
	// tbb::parallel_for(tbb::blocked_range<int>(0, tasks.size()),
	// 	[&](tbb::blocked_range<int> r) {
	// 		for (int i = r.begin(); i < r.end(); ++i) {
	// 			uint x = tasks[i].first;
	// 			uint y = tasks[i].second;
	// 			glm::vec3 color;
	// 			#ifndef SUPER_SAMPLING
	// 			vec3 direction = m_lowerLeftCorner + x*x_axis + (h-y-1)*y_axis;
	// 			Ray ray = Ray(eye, direction);
	// 			color += tracing(ray, root, eye, ambient, lights);
	// 			#else
	// 			for (int sps = 0; sps < super_samples; ++sps)
	// 			{
	// 				float u = static_cast<float>(x + drand48());
	// 				float v = static_cast<float>(y + drand48());
	// 				vec3 direction = m_lowerLeftCorner + u*x_axis + (h-v-1)*y_axis;
	// 				Ray ray = Ray(eye, direction);
	// 				color += tracing(ray, root, eye, ambient, lights);
	// 			}
	// 			color /= static_cast<float>(super_samples);
	// 			#endif
	// 			// Red: 
	// 			image(x, y, 0) = (double)color.r;
	// 			// Green: 
	// 			image(x, y, 1) = (double)color.g;
	// 			// Blue: 
	// 			image(x, y, 2) = (double)color.b;
	// 			counter++;
	// 			if (counter%10000 == 0) {
	// 				cout<<"rendering... ("<<counter<<", "<<total<<") ";
	// 				printf("%0.2f%%",counter/(float)total*100);
	// 				cout<<endl;
	// 			}
	// 		}
	// 	});