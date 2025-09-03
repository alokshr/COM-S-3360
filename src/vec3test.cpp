#include <iostream>
#include <cassert>
#include "vec3.h"

int main(int argc, char** argv) {
    vec3 v1 = vec3();
    vec3 v2 = vec3(1, 4, 5);
    vec3 v3 = vec3(0, 3, -2);

    std::cout << v1 << v2 << v3 << std::endl;

    float product = vec3::dot(v1, v2);
;

    return 0;
}