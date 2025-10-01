#include <iostream>
#include <cassert>
#include "vec3.h"

int main(int argc, char** argv) {
    vec3 v1 = vec3();
    vec3 v2 = vec3(1, 4, 5);
    vec3 v3 = vec3(0, 3, -2);

    std::cout << v1 << v2 << v3 << std::endl;

    assert(vec3::dot(v1, v2) == 0);
    assert(v1 == vec3());
    assert(v2 == vec3(1, 4, 5));
    assert(v3 == vec3(0, 3, -2));
    assert(vec3::cross(vec3(0,3,2), vec3(0, -2, 3)) == vec3(13, 0, 0));
    assert(vec3(0,3,2) * 2 == vec3(0, 6, 4));

    vec3 v4;
    v4 = v2;
    assert(v4 == vec3(1, 4, 5));

    return 0;
}