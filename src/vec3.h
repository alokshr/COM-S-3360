#include <iostream>
#include <cmath>
#ifndef VEC3_H 
#define VEC3_H

#include "mathutils.h"

/**
 * 
 * A class for representing 3D vectors
 */
class vec3 {
    public:
        /**
         * The array that holds the x, y, and z components of this vector
        */
        double e[3];

        /**
         * Creates a new 3D vector with all components set to 0.
         */
        vec3() : e{0,0,0} {};

        /**
         * Creates a new 3D vector with given component values.
         * @param x the x component of the vector
         * @param y the x component of the vector
         * @param z the x component of the vector
         */
        vec3(double x, double y, double z) : e{x,y,z} {};

        /**
         * Creates a new 3D vector by copying the components of another vector
         * @param v vector to copy
         */
        vec3(const vec3& v) {
            e[0] = v[0];
            e[1] = v[1];
            e[2] = v[2];
        }

        /**
         * Copies the components of a vector into this vector
         */
        vec3& operator=(const vec3& v) {
            if (this != &v) {
                e[0] = v[0];
                e[1] = v[1];
                e[2] = v[2];;
            }

            return *this;
        }
        
        /**
         * Returns the x component of this vector
         * @return x component of this vector
         */
        inline double x() const { return e[0]; }

        /**
         * Returns the y component of this vector
         * @return y component of this vector
         */
        inline double y() const { return e[1]; }

        /**
         * Returns the z component of this vector
         * @return z component of this vector
         */
        inline double z() const { return e[2]; }

        /**
         * Applies a scalar of -1 on this vector
         */
        inline vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }

        /**
         * Returns the indexed component of this vector
         */
        inline double operator[](int i) const { return e[i]; }

        /**
         * Returns the indexed component of this vector
         */
        inline double& operator[](int i) { return e[i]; }

        /**
         * Returns the magnitude/distance of this vector.
         * Calculates sqrt(x**2 + y**2 + z**2).
         * @return magnitude of this vector
         */
        inline double mag() const { return std::sqrt(this->sqmag()); }

        /**
         * Returns the squared magnitude/distance of this vector.
         * Calculates x**2 + y**2 + z**2. Faster than mag().
         * @return squared magnitude of this vector
         */
        inline double sqmag() const { return e[0]*e[0] + e[1]*e[1] + e[2]*e[2]; }

        /**
         * Returns a normalized/length one version of this vector.
         * @return normalized version of this vector
         */
        inline vec3 normalize() const {
            return vec3(
                e[0] / this->mag(),
                e[1] / this->mag(),
                e[2] / this->mag()
            );
        }

        /**
         * Calculates the dot product of two vectors.
         * @param lhs vector to dot product
         * @param rhs vector to dot product
         * @return dot product scalar
         */
        static inline double dot(const vec3& lhs, const vec3& rhs) {
            return lhs[0]*rhs[0] + lhs[1]*rhs[1] + lhs[2]*rhs[2];
        }

        /**
         * Calculates the cross product of two vectors.
         * @param lhs vector to cross product
         * @param rhs vector to cross product
         * @return cross product vector
         */
        static inline vec3 cross(const vec3& lhs, const vec3& rhs) {
            return vec3(
                lhs[1]*rhs[2] - lhs[2]*rhs[1],
                lhs[2]*rhs[0] - lhs[0]*rhs[2],
                lhs[0]*rhs[1] - lhs[1]*rhs[0]
            );
        }
        
        /**
         * Calculates the angle in radians between of two vectors.
         * @param lhs starting vector
         * @param rhs ending vector
         * @return angle between vectors in radians
         */
        static inline double angle(const vec3& lhs, const vec3& rhs) {
            return std::acos(vec3::dot(lhs, rhs))/(lhs.mag()*rhs.mag());
        }

        /**
         * Adds the components of a given vector to a vector.
         */
        inline vec3& operator+=(const vec3& v) {
            e[0] += v[0];
            e[1] += v[1];
            e[2] += v[2];
            return *this;
        }

        /**
         * Subtracts the components of a given vector from a vector.
         */
        inline vec3& operator-=(const vec3& v) {
            e[0] -= v[0];
            e[1] -= v[1];
            e[2] -= v[2];
            return *this;
        }

        /**
         * Adds a value to all components of this vector
         */
        inline vec3 operator+=(double a) {
            e[0] += a;
            e[1] += a;
            e[2] += a;
            return *this;
        }
        
        /**
         * Scales this vector by the given scalar value.
         */
        inline vec3 operator*=(double scalar) {
            e[0] *= scalar;
            e[1] *= scalar;
            e[2] *= scalar;
            return *this;
        }

        /**
         * Divide this vector's components by the given scalar value.
         */
        inline vec3 operator/=(double scalar) {
            e[0] /= scalar;
            e[1] /= scalar;
            e[2] /= scalar;
            return *this;
        }

        /**
         * Check for equality based on if vectors have matching components
         */
        inline bool operator==(const vec3& v) {
            return  (e[0]-v[0] == 0.0) &&
                    (e[1]-v[1] == 0.0) &&
                    (e[2]-v[2] == 0.0);
        }

        /**
         * Check for inequality based on if vectors have different components
         */
        inline bool operator!=(const vec3& v) {
            return !(*this == v);
        }

        /**
         * Returns whether or not this vector is close to 0 length
         * @return true if close to 0 length, false otherwise
         */
        inline bool near_zero() const {
            const double s = 1e-8;
            return 
                std::fabs(e[0] < s) &&
                std::fabs(e[1] < s) && 
                std::fabs(e[2] < s);
        }
};

/**
 * Prints out a vector
 */
std::ostream& operator<<(std::ostream& os, const vec3& v) {
    os << "(" << v[0] << ", " << v[1] << ", " << v[2] << ")";
    return os;
}

/**
 * Adds the components of two vectors as a new vector.
 */
inline vec3 operator+(const vec3& lhs, const vec3& rhs) {
    return vec3(lhs[0] + rhs[0],
                lhs[1] + rhs[1],
                lhs[2] + rhs[2]);
}

/**
 * Subtracts the components of two vectors as a new vector.
 */
inline vec3 operator-(const vec3& lhs, const vec3& rhs) {
    return vec3(lhs[0] - rhs[0],
                lhs[1] - rhs[1],
                lhs[2] - rhs[2]);
}


/**
 * Scales a vector by the given scalar value.
 */
inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u[0]*v[0],
                u[1]*v[1],
                u[2]*v[2]);
}

/**
 * Adds a value to all components of a vector
 */
inline vec3 operator+(const vec3& v, double a) {
    return vec3(v[0]+a, v[1]+a, v[2]+a);
}

/**
 * Adds a value to all components of a vector
 */
inline vec3 operator+(double a, const vec3& v) {
    return v + a;
}

/**
 * Scales a vector by the given scalar value.
 */
inline vec3 operator*(const vec3& v, double scalar) {
    return vec3(v[0]*scalar,
                v[1]*scalar,
                v[2]*scalar);
}

/**
 * Scales a vector by the given scalar value.
 */
inline vec3 operator*(double scalar, const vec3& v) {
    return v * scalar;
}

/**
 * Divides a vector's components by the given scalar value.
 */
inline vec3 operator/(const vec3& v, double scalar) {
    return vec3(v[0]/scalar,
                v[1]/scalar,
                v[2]/scalar);
}

/**
 * Divides a vector's components by the given scalar value.
 */
inline vec3 operator/(double scalar, const vec3& v) {
    return v / scalar;
}

/**
 * Returns a vector with random x, y, and z components between 0 and 1 
 * @return uniformly sampled vector
 */
inline vec3 randvec3() {
    return vec3(random_double(), random_double(), random_double());
}

/**
 * Returns a vector with random x, y, and z compoenents between a given min and max
 * @param min lower bound
 * @param max upper bound
 * @return uniformly sampled vector
 */
inline vec3 randvec3(double min, double max) {
    return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
}


/**
 * Returns a vector with random x, y, and z components sampled from a normal distribution 
 * @return normally sampled vector
 */
inline vec3 randvec3_normal() {
    return vec3(random_normal(), random_normal(), random_normal());
}

/**
 * Returns a uniformly distributed point on a unit hemisphere given a normal vector
 * @param normal direction of hemisphere
 * @return random point on hemisphere
 */
inline vec3 randvec3_on_hemisphere(const vec3& normal) {
    vec3 on_unit_sphere = randvec3_normal().normalize();
    return (vec3::dot(on_unit_sphere, normal) > 0.0) ? on_unit_sphere : -on_unit_sphere;
}

/**
 * Returns a cosine sampled point on a hemisphere pointed in the Z axis
 * @return cosine sampled point on hemisphere
 */
inline vec3 random_cosine_direction() {
    auto r1 = random_double();
    auto r2 = random_double();

    auto phi = 2*M_PI*r1;
    auto x = std::cos(phi) * std::sqrt(r2);
    auto y = std::sin(phi) * std::sqrt(r2);
    auto z = std::sqrt(1-r2);

    return vec3(x, y, z);
}

/**
 * Returns a linearly interpolated vector between two vectors
 * @param a starting vector
 * @param b ending vector
 * @param t percentage between vectors
 * @return linearly interpolated vector
 */
vec3 lerp(vec3 a, vec3 b, double t) {
    return (1-t)*a + t*b;
}

/**
 * Returns a reflected vector given an incoming vector and a normal vector
 * @param incoming incoming vector to reflect
 * @param normal direction of reflection surface
 * @return reflected vector
 */
inline vec3 reflect(const vec3& incoming, const vec3& normal) {
    return incoming - 2*vec3::dot(incoming, normal)*normal;
}

/**
 * Returns the refraction of a vector given an incoming vector, a normal vector, and the ratio between refractive indicies
 * @param incoming incoming vector to refract
 * @param normal direction of refraction surface
 * @param refractive_ratio the ratio between the refractive indicies of the incoming medium and the refracting medium 
 * @return refracted vector
 */
inline vec3 refract(const vec3& incoming, const vec3& normal, double refractive_ratio) {
    double cos_theta = std::fmin(vec3::dot(-incoming, normal), 1.0);
    vec3 r_out_perp =  refractive_ratio * (incoming + cos_theta*normal);
    vec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.sqmag())) * normal;
    return r_out_perp + r_out_parallel;
}
#endif