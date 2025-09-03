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

float vec3::dot(const vec3& lhs, const vec3& rhs) {
    return (lhs.x * rhs.x) +
        (lhs.y * rhs.y) +
        (lhs.z * rhs.z);
}

vec3 vec3::cross(const vec3& lhs, const vec3& rhs) {
    return vec3(
        lhs.y*rhs.z - lhs.z*rhs.y,
        lhs.z*rhs.x - lhs.x*rhs.z,
        lhs.x*rhs.y - lhs.y*rhs.x
    );
}

vec3 vec3::operator+(const vec3& v) {
    return vec3(this->x + v.x,
                this->y + v.y,
                this->z + v.z);
}

vec3 vec3::operator-(const vec3& v) {
    return vec3(this->x - v.x,
                this->y - v.y,
                this->z - v.z);
}

vec3 vec3::operator*(const int scalar) {
    return vec3(this->x * scalar,
                this->y * scalar,
                this->z * scalar);
}

vec3 vec3::operator*(const float scalar) {
    return vec3(this->x * scalar,
                this->y * scalar,
                this->z * scalar);
}

vec3 vec3::operator/(const int scalar) {
    return vec3(this->x / scalar,
                this->y / scalar,
                this->z / scalar);
}

vec3 vec3::operator/(const float scalar) {
    return vec3(this->x / scalar,
                this->y / scalar,
                this->z / scalar);
}

bool vec3::operator==(const vec3& v) {
    return (this->x == v.x) &&
            (this->y == v.y) &&
            (this->z == v.z);
}


bool vec3::operator!=(const vec3& v) {
    return (this->x != v.x) ||
            (this->y != v.y) ||
            (this->z != v.z);
}

std::ostream& operator<<(std::ostream& os, const vec3& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}