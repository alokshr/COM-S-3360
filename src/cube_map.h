#ifndef CUBE_MAP_H
#define CUBE_MAP_H

#include "collidable.h"
#include "texture.h"
#include "mathutils.h"
#include "image.h"

#include <array>

enum cube_face {
    POS_X, NEG_X,
    POS_Y, NEG_Y,
    POS_Z, NEG_Z
};

/**
 * A class for rendering cube map environments
 */
class cube_map {
    public:
    
        /**
         * Creates a cube map with a given texture repeated for all faces
         * @param tex texture of each face
         */
        cube_map(shared_ptr<texture> tex) {
            for (int i = 0; i < 6; i++) {
                faces[i] = tex;
            }
        }

        /**
         * Creates a cube map with given 6 texture faces in the order
         * +X, -X, +Y, -Y, +Z, -Z
         * @param faces faces of cube map
         */
        cube_map(shared_ptr<texture> (&src_faces)[6]) {
            for (int i = 0; i < 6; i++) {
                faces[i] = src_faces[i];
            }
        }
        
        /**
         * Creates a cube map with given a tex_image of a horizontal cross cube map image
         * @param faces faces of cube map
         */
        cube_map(const tex_image& cross) {
            int face_width = cross.width() / 4;
            int face_height = cross.height() / 3;

            tex_image pos_x(cross, 2*face_width, 1*face_height, face_width, face_height);
            tex_image neg_x(cross, 0*face_width, 1*face_height, face_width, face_height);

            tex_image pos_y(cross, 1*face_width, 2*face_height, face_width, face_height);
            tex_image neg_y(cross, 1*face_width, 0*face_height, face_width, face_height);

            tex_image pos_z(cross, 1*face_width, 1*face_height, face_width, face_height);
            tex_image neg_z(cross, 3*face_width, 1*face_height, face_width, face_height);

            faces[POS_X] = make_shared<image_texture>(pos_x);
            faces[NEG_X] = make_shared<image_texture>(neg_x);
            faces[POS_Y] = make_shared<image_texture>(pos_y);
            faces[NEG_Y] = make_shared<image_texture>(neg_y);
            faces[POS_Z] = make_shared<image_texture>(pos_z);
            faces[NEG_Z] = make_shared<image_texture>(neg_z);
        }

        /**
         * Returns the color value in a given direction
         * @param r ray to hit background
         */
        color value(const ray& r) const {
            // Normalize direction vector
            vec3 dir = r.direction().normalize();

            // Find largest axis
            double x = dir.x();
            double y = dir.y();
            double z = dir.z();

            double ax = std::fabs(x);
            double ay = std::fabs(y);
            double az = std::fabs(z);

            double u, v;

            int face;

            // Update uv values based on largest axis
            if (ax >= ay && ax >= az) { // X axis
                if (x > 0) { // +X
                    u = -z / ax;
                    v =  y / ax;
                    face = POS_X;
                } else { // -X
                    u =  z / ax;
                    v =  y / ax;
                    face = NEG_X;
                }
            } else if (ay >= ax && ay >= az) { // Y axis
                if (y > 0) { // +Y
                    u =  x / ay;
                    v = -z / ay;
                    face = POS_Y;
                } else { // -Y
                    u =  x / ay;
                    v =  z / ay;
                    face = NEG_Y;
                }
            } else { // Z axis
                if (z > 0) { // +Z
                    u =  x / az;
                    v =  y / az;
                    face = POS_Z;
                } else { // -Z
                    u = -x / az;
                    v =  y / az;
                    face = NEG_Z;
                }
            }

            // Map from -1-1 to 0-1
            u = 0.5 * (u + 1.0);
            v = 0.5 * (v + 1.0);

            return faces[face]->value(u, v, r.at(0));
        }
    private:
        /**
         * The texture faces of this cube map
         */
        shared_ptr<texture> faces[6];
};


#endif