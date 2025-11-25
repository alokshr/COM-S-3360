#ifndef IMAGE_H
#define IMAGE_H

#include "renderlib.h"
#include <fstream>
#include <vector>

using image = std::vector<std::vector<color>>;

/**
 * Takes a 2D array of float colors ranging from [0, 1] and outputs a .ppm file 
 * @return 0 on success, -1 on failure
 * @param img an image struct
 * @param filename the name to save the .ppm file as
 */
int output_ppm_image(image img, const char* filename) {
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
    
    std::cout << "Successfully saved to " << filename << std::endl;
    return 0;
}

#endif