#include "vec3.h"
#include <cmath>

vec3::vec3() {
    this->x = 0;
    this->y = 0;
    this->z = 0;
}

vec3::vec3(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

float vec3::mag() {
    return sqrt(this->sqmag());
};

float vec3::sqmag() {
    return pow(this->x, 2) +
            pow(this->y, 2) +
            pow(this->z, 2);
};

vec3 vec3::operator+(const vec3& other) {
    return vec3(this->x + other.x,
                this->y + other.y,
                this->z + other.z);
}

vec3 vec3::operator-(const vec3& other) {
    return vec3(this->x - other.x,
                this->y - other.y,
                this->z - other.z);
}

vec3 vec3::operator*(const int other) {
    return vec3(this->x * other,
                this->y * other,
                this->z * other);
}

vec3 vec3::operator*(const float other) {
    return vec3(this->x * other,
                this->y * other,
                this->z * other);
}

vec3 vec3::operator/(const int other) {
    return vec3(this->x / other,
                this->y / other,
                this->z / other);
}

vec3 vec3::operator/(const float other) {
    return vec3(this->x / other,
                this->y / other,
                this->z / other);
}