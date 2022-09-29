#ifndef __MY_GLM_UTILS__
#define __MY_GLM_UTILS__

using namespace std;

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
#include <algorithm>
using namespace glm;

mat4 myRotate(double theta, vec4 axis) {

    vec3 axis3 = normalize(vec3(axis));

    double ux = axis3[0];
    double uy = axis3[1];
    double uz = axis3[2];

    mat4 res(1.0f);

    res[0][0] = cos(theta) + ux*ux*(1-cos(theta));
    res[0][1] = uy*ux*(1-cos(theta)) + uz*sin(theta);
    res[0][2] = uz*ux*(1-cos(theta)) - uy*sin(theta);

    res[1][0] = ux*uy*(1-cos(theta)) - uz*sin(theta);
    res[1][1] = cos(theta) + uy*uy*(1-cos(theta));
    res[1][2] = uz*uy*(1-cos(theta)) + ux*sin(theta);

    res[2][0] = ux*uz*(1-cos(theta))+uy*sin(theta);
    res[2][1] = uy*uz*(1-cos(theta))-ux*sin(theta);
    res[2][2] = cos(theta) + uz*uz*(1-cos(theta));

    return res;
}




#endif