#ifndef IMAGE_H
#define IMAGE_H

#include "renderlib.h"
#include <fstream>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "lib/stb_image.h"

using image = std::vector<std::vector<color>>;

/**
 * Takes a 2D array of float colors ranging from [0, 1] and outputs a .ppm file 
 * @return 0 on success, -1 on failure
 * @param img an image struct
 * @param filename the name to save the .ppm file as
 */
int output_ppm_image(image img, const std::string& filename) {
    if (img.size() < 1 || img[0].size() < 1) {
        std::cerr << "Cannot print image of zero width or height" << std::endl;
        return -1;
    }

    std::ofstream file;

    file.open(filename, std::ios::out | std::ios::trunc);

    if (!file.is_open()) {
        std::cerr << "Unable to open " << filename << std::endl;
        return -1;
    }
    
    file << "P3 " << img[0].size() << " " << img.size() << " 255\n";

    for (int y = 0; y < img.size(); y++) {
        for (int x = 0; x < img[0].size(); x++) {
            color c = color_float_to_rgb(img[y][x]);
            
            file << c[0] << " " << c[1] << " " << c[2] << " ";
        }
    }

    file.close();
    
    std::clog << "Successfully saved to " << filename << std::endl;
    return 0;
}

class tex_image {
    public:
    
        /**
         * Creates an empty tex_image to store and load image data
         */
        tex_image() {}

        /**
         * Creates a tex_image loaded with the given file's image data
         * @param filename name of image file to load
         */
        tex_image(const std::string filename) {
            if (load(filename)) return;

            std::cerr << "Unable to load " << filename << " as texture image\n";
        }

        ~tex_image() {
            delete[] bdata;
            STBI_FREE(fdata);
        }

        /**
         * Returns this image's width
         * @return image width, 0 if no image was loaded
         */
        int width() const { return (fdata == nullptr) ? 0 : image_width; }
        
        /**
         * Returns this image's height
         * @return image height, 0 if no image was loaded
         */
        int height() const { return (fdata == nullptr) ? 0 : image_height; }

        /**
         * Returns the address of the 3-byte RGB pixel at a given x and y,
         * with (0, 0) being the bottom-left of the image
         * @param x x coord on image
         * @param y y coord on image
         * @return 3-byte RGB value, returns a magenta color if no image data is found
         */
        const unsigned char* get_pixel(int x, int y) const {
            // Return magenta if no image data is found
            static unsigned char magenta[3] = {255, 0, 255};
            if (bdata == nullptr) return magenta;

            x = clamp(x, 0, image_width-1);
            y = clamp(y, 0, image_height-1);

            return bdata + y*bytes_per_row + x*bytes_per_pixel;
        }

        /**
         * Attempts to load the given filename's image data into this tex_image
         * @param filename name of image file to load
         * @return true if image was loaded successfully, false if image was not loaded
         */
        bool load(std::string filename) {
            // Var for collecting number of channels per pixel, not used
            int n = bytes_per_pixel;

            fdata = stbi_loadf(filename.c_str(), &image_width, &image_height, &n, bytes_per_pixel);
            if (fdata == nullptr) return false;

            bytes_per_row = image_width * bytes_per_pixel;
            convert_to_bytes();
            return true;
        }

    private:
        /**
         * The number of bytes to load 
         */
        const int bytes_per_pixel = 3;

        /**
         * The pointer to the floating-point color values ranging from 0.0-1.0 from the loaded image
         */
        float *fdata = nullptr;
        
        /**
         * The pointer to the 3-byte RGB color values ranging from 0-255 from the loaded image
         */
        unsigned char *bdata = nullptr;

        /**
         * The width of this image
         */
        int image_width = 0;

        /**
         * The height of this image
         */
        int image_height = 0;

        /**
         * The number of bytes per row of the image
         */
        int bytes_per_row = 0;

        /**
         * Converts this image's floating point image data and stores in it bdata
         */
        void convert_to_bytes() {
            int total_bytes = image_width * image_height * bytes_per_pixel;

            bdata = new unsigned char[total_bytes];

            unsigned char* bptr = bdata;
            float* fptr = fdata;
            for (int i=0; i < total_bytes; i++, fptr++, bptr++)
                *bptr = float_to_byte(*fptr);
            }

        /**
         * Returns the given float ranging from 0.0-1.0 to a byte value ranging from0-255
         * @param v float to convert
         * @return converted byte value
         */
        static unsigned char float_to_byte(float v) {
            // Ensure float value stays bounded
            if (v <= 0.0) return 0;
            if (v >= 1.0) return 255;
            return static_cast<unsigned char>(256.0 * v);
        }
};

#endif