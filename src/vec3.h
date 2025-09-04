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

        /**
         * The x component of a vec3.
         */
        float x;

        /**
         * The y component of a vec3.
         */
        float y;

        /**
         * The z component of a vec3.
         */
        float z;

        /**
         * Creates a new 3D vector with all components set to 0.
         */
        vec3() : x(0),y(0),z(0) {};

        /**
         * Creates a new 3D vector with given component values.
         * @param x the x component of the vector
         * @param y the x component of the vector
         * @param z the x component of the vector
         */
        vec3(float x, float y, float z) : x(x), y(y), z(z) {};

        vec3(const vec3& rhs);

        vec3& operator=(const vec3& rhs);

        /**
         * Returns the magnitude/distance of this vector.
         * Calculates sqrt(x**2 + y**2 + z**2).
         */
        inline float mag() const {
            return sqrt(this->sqmag());
        }

        /**
         * Returns the squared magnitude/distance of this vector.
         * Calculates x**2 + y**2 + z**2. Faster than mag().
         */
        inline float sqmag() const {    
            return pow(this->x, 2) +
                pow(this->y, 2) +
                pow(this->z, 2);
        }

        /**
         * Normalizes this vector to have a magnitude of one.
         */
        inline void norm() {
            *this /= this->mag();
        }

        /**
         * Calculates the dot product of two vectors.
         */
        static inline float dot(const vec3& lhs, const vec3& rhs) {
            return (lhs.x * rhs.x) +
                (lhs.y * rhs.y) +
                (lhs.z * rhs.z);
        }

        /**
         * Calculates the cross product of two vectors.
         */
        static inline vec3 cross(const vec3& lhs, const vec3& rhs) {
            return vec3(
                lhs.y*rhs.z - lhs.z*rhs.y,
                lhs.z*rhs.x - lhs.x*rhs.z,
                lhs.x*rhs.y - lhs.y*rhs.x
            );
        }
        
        /**
         * Calculates the angle in radians between of two vectors.
         */
        static inline float angle(const vec3& lhs, const vec3& rhs) {
            return acos(vec3::dot(lhs, rhs))/(lhs.mag()*rhs.mag());
        }

        /**
         * Adds the components of two vectors as a new vector.
         */
        vec3 operator+(const vec3& v);
        
        /**
         * Adds the components of a given vector to a vector.
         */
        vec3& operator+=(const vec3& v);

        /**
         * Subtracts the components of two vectors as a new vector.
         */
        vec3 operator-(const vec3& v);

        /**
         * Subtracts the components of a given vector from a vector.
         */
        vec3& operator-=(const vec3& v);

        /**
         * Scales a vector by the given scalar value.
         */
        vec3 operator*(double scalar);
        
        /**
         * Scales this vector by the given scalar value.
         */
        vec3 operator*=(double scalar);
        
        /**
         * Divide a vector's components by the given scalar value.
         */
        vec3 operator/(double scalar);

        /**
         * Divide this vector's components by the given scalar value.
         */
        vec3 operator/=(double scalar);

        /**
         * Check for equality based on if vectors have matching components
         * @param v vec3 to compare
         */
        bool operator==(const vec3& v);
        
        /**
         * Check for inequality based on if vectors have different components
         * @param v vec3 to compare
         */
        bool operator!=(const vec3& v);
};

std::ostream& operator<<(std::ostream& os, const vec3& v);

#endif