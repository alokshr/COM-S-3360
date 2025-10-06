#include <iostream>
#include <cmath>
#ifndef VEC3_H 
#define VEC3_H

/**
 * 
 * A class for representing 3D vectors
 */
class vec3 {
    public:
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
         * @param v vector to copy
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
         */
        inline double x() const { return e[0]; }

        /**
         * Returns the y component of this vector
         */
        inline double y() const { return e[1]; }

        /**
         * Returns the z component of this vector
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
         */
        inline double mag() const { return std::sqrt(this->sqmag()); }

        /**
         * Returns the squared magnitude/distance of this vector.
         * Calculates x**2 + y**2 + z**2. Faster than mag().
         */
        inline double sqmag() const { return e[0]*e[0] + e[1]*e[1] + e[2]*e[2]; }

        /**
         * Returns a normalized/length one version of this vector.
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
         */
        static inline double dot(const vec3& lhs, const vec3& rhs) {
            return lhs[0]*rhs[0] + lhs[1]*rhs[1] + lhs[2]*rhs[2];
        }

        /**
         * Calculates the cross product of two vectors.
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
         * @param v vec3 to compare
         */
        inline bool operator==(const vec3& v) {
            return  (e[0]-v[0] == 0.0) &&
                    (e[1]-v[1] == 0.0) &&
                    (e[2]-v[2] == 0.0);
        }

        /**
         * Check for inequality based on if vectors have different components
         * @param v vec3 to compare
         */
        inline bool operator!=(const vec3& v) {
            return !(*this == v);
        }
};

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
 * Divide a vector's components by the given scalar value.
 */
inline vec3 operator/(const vec3& v, double scalar) {
    return vec3(v[0]/scalar,
                v[1]/scalar,
                v[2]/scalar);
}

/**
 * Divide a vector's components by the given scalar value.
 */
inline vec3 operator/(double scalar, const vec3& v) {
    return v / scalar;
}
#endif