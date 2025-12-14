#ifndef ONB_H
#define ONB_H

#include "vec3.h"

/**
 * A class for creating orthonormal basis vectors
 */
class onb {
    public:
        /**
         * Creates orthonormal basis vectors from a given vector representing the k/z basis vector
         * @param k vector representing the k/z basis vector
         */
        onb(const vec3& k) {
            axis[2] = k.normalize();
            vec3 a = (std::fabs(axis[2].x()) > 0.9) ? vec3(0,1,0) : vec3(1,0,0);
            axis[1] = vec3::cross(axis[2], a).normalize();
            axis[0] = vec3::cross(axis[2], axis[1]);
        }

        /**
         * Returns the i basis vector
         * @return i basis
         */
        const vec3& i() const { return axis[0]; }
        
        /**
         * Returns the j basis vector
         * @return j basis
         */
        const vec3& j() const { return axis[1]; }
        
        /**
         * Returns the k basis vector
         * @return k basis
         */
        const vec3& k() const { return axis[2]; }

        /**
         * Transforms a vector from basis coordinates to this onb's space 
         * @param v vector to convert
         * @return vector converted to this onb's space
         */
        vec3 transform(const vec3& v) const {
            return (v[0] * axis[0]) + (v[1] * axis[1]) + (v[2] * axis[2]);
        }

    private:
        /**
         * The orthonormal i,j,k basis vectors 
         */
        vec3 axis[3];
};
#endif