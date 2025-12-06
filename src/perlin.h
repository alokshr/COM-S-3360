#ifndef PERLIN_H
#define PERLIN_H

#include "vec3.h"
#include "mathutils.h"

class perlin {
    public:
        perlin() {
            // Initialize random vectors
            for (int i = 0; i < point_count; i++) {
                randfloat[i] = random();
            }

            // Permute random vectors for each axis
            perlin_generate_perm(perm_x);
            perlin_generate_perm(perm_y);
            perlin_generate_perm(perm_z);
        }

        double noise(const vec3& p) const {
            auto i = int(4*p.x()) & 255;
            auto j = int(4*p.y()) & 255;
            auto k = int(4*p.z()) & 255;

            return randfloat[perm_x[i] ^ perm_y[j] ^ perm_z[k]];
        }

    private:
        static const int point_count = 256;
        double randfloat[point_count];
        int perm_x[point_count];
        int perm_y[point_count];
        int perm_z[point_count];

        static void perlin_generate_perm(int* p) {
            for (int i = 0; i < point_count; i++)
                p[i] = i;

            permute(p, point_count);
        }

        static void permute(int* p, int n) {
            for (int i = n-1; i > 0; i--) {
                int target = random_int(0, i);
                int tmp = p[i];
                p[i] = p[target];
                p[target] = tmp;
            }
    }
};

#endif