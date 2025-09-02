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
         * Creates a vec3 with all components set to 0.
         */
        vec3();

        /**
         * Creates a vec3 with given component values.
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

        vec3 operator+(const vec3& other);
        vec3 operator-(const vec3& other);

        /**
         * Scales this vector by the given scalar value.
         */
        vec3 operator*(const int other);
        
        /**
         * Scales this vector by the given scalar value.
         */
        vec3 operator*(const float other);
        
        /**
         * Scales this vector by the given scalar value.
         */
        vec3 operator/(const int other);
        
        /**
         * Scales this vector by the given scalar value.
         */
        vec3 operator/(const float other);
};

#endif