#ifndef QUATERNION_H
#define QUATERNION_H

#include "vec3.h"
#include "mathutils.h"

class quaternion {
    public:
        double s;
        vec3 v;
        quaternion() {};
        quaternion(double s, vec3 v): s(s), v(v) {}

        /**
         * Multiplies two quaternions
         * 
         * pq = <s_p*a_q - v_p.v_q, v_p x v_q + v_p*s_q + s_q*v_p>
         */
        quaternion operator*(const quaternion& q) const {
            return quaternion(
                s*q.s - vec3::dot(v, q.v),
                vec3::cross(v, q.v) + (s*q.v)
            );
        }
};

void rotate(vec3& v, vec3 axis, double degrees) {
    // Rotation = <cos(theta/2), x sin(t/2), y sin(t/2), z sin(t/2)>
    double t = d2r(degrees);
    double sint2 = sin(t/2.0);

    quaternion q = quaternion(
        cos(t/2.0),
        vec3(
            axis[0] * sint2,
            axis[1] * sint2,
            axis[2] * sint2
        )
    );

    quaternion p = quaternion(
        1,
        v
    );

    quaternion qinv = quaternion(
        1,
        -q.v
    );

    // Inversion of a quaternion: p = <s, x, y, z>, p^-1 = <s, -x, -y, -z>
    // Rotation p' = q^-1 * p * q
    // Reversing p = q * p' * q^-1
    quaternion pprime = q*p*qinv;

    v = pprime.v;
}
#endif