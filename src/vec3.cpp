#include "vec3.h"
#include <cmath>

vec3::vec3(const vec3& rhs) {
    this->x = rhs.x;
    this->y = rhs.y;
    this->z = rhs.z;
}

vec3& vec3::operator=(const vec3& rhs) {
    if (this != &rhs) {
        this->x = rhs.x;
        this->y = rhs.y;
        this->z = rhs.z;
    }

    return *this;
}

vec3 vec3::operator+(const vec3& v) {
    return vec3(this->x + v.x,
                this->y + v.y,
                this->z + v.z);
}

vec3& vec3::operator+=(const vec3& v) {
    this->x += v.x;
    this->y += v.y;
    this->z += v.z;

    return *this;
}

vec3 vec3::operator-(const vec3& v) {
    return vec3(this->x - v.x,
                this->y - v.y,
                this->z - v.z);
}

vec3& vec3::operator-=(const vec3& v) {
    this->x -= v.x;
    this->y -= v.y;
    this->z -= v.z;

    return *this;
}

vec3 vec3::operator*(double scalar) {
    return vec3(this->x * scalar,
                this->y * scalar,
                this->z * scalar);
}

vec3 vec3::operator*=(double scalar) {
    this->x *= scalar;
    this->y *= scalar;
    this->z *= scalar;

    return *this;
}

vec3 vec3::operator/(double scalar) {
    return vec3(this->x / scalar,
                this->y / scalar,
                this->z / scalar);
}

vec3 vec3::operator/=(double scalar) {
    this->x /= scalar;
    this->y /= scalar;
    this->z /= scalar;
            
    return *this;
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