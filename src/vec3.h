#include <iostream>
#ifndef VEC3_H 
#define VEC3_H


/**
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
        vec3();

        /**
         * Creates a new 3D vector with given component values.
         * @param x the x component of the vector
         * @param y the x component of the vector
         * @param z the x component of the vector
         */
        vec3(float x, float y, float z);

        /**
         * Returns the magnitude/distance of this vector.
         * Calculates sqrt(x**2 + y**2 + z**2).
         */
        float mag();

        /**
         * Returns the squared magnitude/distance of this vector.
         * Calculates x**2 + y**2 + z**2. Faster than mag().
         */
        float sqmag();

        /**
         * Calculates the dot product of two vectors.
         */
        static float dot(const vec3& lhs, const vec3& rhs);

        /**
         * Calculates the cross product of two vectors.
         */
        static vec3 cross(const vec3& lhs, const vec3& rhs);

        /**
         * Adds the components of two vectors as a new vector.
         */
        vec3 operator+(const vec3& v);

        /**
         * Subtracts the components of two vectors as a new vector.
         */
        vec3 operator-(const vec3& v);

        /**
         * Scales this vector by the given scalar value.
         */
        vec3 operator*(const int scalar);
        
        /**
         * Scales this vector by the given scalar value.
         */
        vec3 operator*(const float scalar);
        
        /**
         * Scales this vector by the given scalar value.
         */
        vec3 operator/(const int scalar);
        
        /**
         * Scales this vector by the given scalar value.
         */
        vec3 operator/(const float scalar);

        bool operator==(const vec3& v);
        
        bool operator!=(const vec3& v);
};

std::ostream& operator<<(std::ostream& os, const vec3& v);

#endif