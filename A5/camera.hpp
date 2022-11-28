#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Ray.hpp"

using namespace glm;

vec3 random_in_unit_disk() {
    vec3 p;
    do {
        p = 2.0 * vec3(drand48(), drand48(), 0) - vec3(1, 1, 0);
    } while (dot(p, p) >= 1);
    return p;
}

class camera {
public:
    camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperture, float focus_dist) { // vfov is to to bottom in degrees
        lens_radius = aperture / 2;
        float theta = vfov * M_PI / 180;
        float half_height = tan(theta / 2);
        float half_width = aspect * half_height;
        origin = lookfrom;
        w = normalize(lookfrom - lookat);
        u = normalize(cross(vup, w));
        v = cross(w, u);

        lower_left_corner = origin - half_height * focus_dist * v  - half_width * focus_dist * u - w * focus_dist;
        horizontal = 2 * half_width * focus_dist * u;
        vertical = 2 * half_height * focus_dist * v;
    }

    Ray get_ray(float s, float t) {
        vec3 rd = random_in_unit_disk() * lens_radius;
        vec3 offset = rd.x * u + rd.y * v;
        // 以左下角坐标为基础，根据分割的u、v值映射所有范围内的点
        return Ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset);
    }
private:
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 origin;
    vec3 u, w, v;
    float lens_radius;
};