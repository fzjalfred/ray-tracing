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

mat4 myPerspectiveDownZ(double aspect, double FOV,
float near,
float far) {
    mat4 res (1.0f);
    res[0][0] = aspect*(1/tan(FOV/2));
    res[1][1] = 1/tan(FOV/2);
    // res[0][0] = 1;
    // res[1][1] = 1;
    res[2][2] = -(far + near)/(far - near);
    //res[2][2] = far/(far - near);
    res[2][3] = -1;
    res[3][2] = - 2*(far*near)/(far - near); 
    //res[3][2] = - (far*near)/(far - near); 

    return res;
}

mat4 myPerspectiveUpZ(double aspect, double FOV,
float near,
float far) {
    mat4 res (1.0f);
    res[0][0] = aspect*(1/tan(FOV/2));
    res[1][1] = 1/tan(FOV/2);
    // res[0][0] = 1;
    // res[1][1] = 1;
    res[2][2] = (far + near)/(far - near);
    //res[2][2] = far/(far - near);
    res[2][3] = 1;
    res[3][2] = - 2*(far*near)/(far - near); 
    //res[3][2] = - (far*near)/(far - near); 

    return res;
}


bool clip(vec2& A, vec2& B, vec2 P, vec2 n) {
    auto l = [=] (vec2 x) {
        return dot((x - P), n);
    };
    float wecA = dot((A-P), n);
    float wecB = dot((B-P), n);
    if ( wecA < 0 && wecB < 0 ) return false;
    if ( wecA >= 0 && wecB >= 0 ) return true;
    float t = wecA / (wecA - wecB);
    if ( wecA < 0 )
        A = A + t*(B-A);
    else
        B = A + t*(B-A);
    return true;
}

bool clip(vec3& A, vec3& B, vec3 P, vec3 n) {
    float wecA = dot((A-P), n);
    float wecB = dot((B-P), n);
    // if (n[2] == 1) {
    //     cout<<"A: "<<A[0]<<" "<<A[1]<<" "<<A[2]<<endl;
    //     cout<<"P: "<<P[0]<<" "<<P[1]<<" "<<P[2]<<endl;
    //     cout<<"wecA: "<<wecA<<endl;
    //     cout<<"wecB: "<<wecB<<endl;
    // }
    if ( wecA < 0 && wecB < 0 ) return false;
    if ( wecA >= 0 && wecB >= 0 ) return true;
    float t = wecA / (wecA - wecB);
    if ( wecA < 0 )
        A = A + t*(B-A);
    else
        B = A + t*(B-A);
    return true;
}


#endif