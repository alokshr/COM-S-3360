#ifndef OBJ_PARSER_H
#define OBJ_PARSER_H

#include "vec3.h"
#include "collidable_list.h"
#include "triangle.h"
#include "material.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <deque>

namespace fs = std::filesystem;

struct face_vertex {
    int vertex_index = -1;
    int uv_index = -1;
    int normal_index = -1;
};

struct face_group {
    std::vector<face_vertex> face_verts;
    std::string name;

    inline bool is_empty() { return face_verts.size() == 0; }
};

class obj_parser {
    public:
        obj_parser() {}

        void parse_obj_file(const std::string& filename) {
            fs::path filepath = filename;

            if (filepath.extension() != ".obj") {
                std::cerr << "Unable to load " << filename << ": non-obj file" << std::endl;
                return;
            }

            std::ifstream file(filename);

            if (!file.is_open()) {
                std::cerr << "Unable to load " << filename << std::endl;
                return;
            }

            std::string line;
            
            // Start with first face group
            face_groups.emplace_back();
            face_group* current_group = &face_groups.back();

            // Process each line of the .obj file
            while (getline(file, line)) {
                std::istringstream stream(line);
                std::string type;
                stream >> type;

                // Figure out what to do based on the type of data
                // Vertex data
                if (type == "v") {
                    double x, y, z;
                    stream >> x >> y >> z;
                    verts.emplace_back(x, y, z);
                    if (!current_group->is_empty()) {
                        face_groups.emplace_back();
                        current_group = &face_groups.back();
                    }
                // Vertex texture data
                } else if (type == "vt") {
                    double u, v;
                    stream >> u >> v;
                    texs.emplace_back(u, v, 0);
                // Vertex normal data
                } else if (type == "vn") {
                    double x, y, z;
                    stream >> x >> y >> z;
                    texs.emplace_back(x, y, z);
                // Face data
                } else if (type == "f") {
                    std::vector<std::string> clusters;
                    std::string index_cluster;
                    while (stream >> index_cluster) {
                        clusters.push_back(index_cluster);
                    }
                    process_face(clusters, current_group->face_verts);
                // Group data
                } else if (type == "g") {
                    if (!current_group->is_empty()) {
                        face_groups.emplace_back();
                        current_group = &face_groups.back();
                    }
                    stream >> current_group->name;
                }
            }

            file.close();
        }
    
        shared_ptr<collidable_list> generate_triangles(shared_ptr<material> mat) {
            shared_ptr<collidable_list> objects = make_shared<collidable_list>();
            
            for (const auto& face_group : face_groups) {
                shared_ptr<collidable_list> object = make_shared<collidable_list>();
                
                for (size_t n = 0; n < face_group.face_verts.size(); n += 3) {
                    int v0_idx = face_group.face_verts.at(n).vertex_index;
                    int v1_idx = face_group.face_verts.at(n+1).vertex_index;
                    int v2_idx = face_group.face_verts.at(n+2).vertex_index;

                    int vn0_idx = face_group.face_verts.at(n).normal_index;
                    int vn1_idx = face_group.face_verts.at(n+1).normal_index;
                    int vn2_idx = face_group.face_verts.at(n+2).normal_index;

                    shared_ptr<triangle> tri;

                    if (vn0_idx != -1 &&
                        vn1_idx != -1 &&
                        vn2_idx != -1) {
                        tri = make_shared<triangle>(
                            verts.at(v0_idx),
                            verts.at(v1_idx),
                            verts.at(v2_idx),
                            norms.at(vn0_idx),
                            norms.at(vn1_idx),
                            norms.at(vn2_idx),
                            mat
                        );
                    } else {
                        tri = make_shared<triangle>(
                            verts.at(v0_idx),
                            verts.at(v1_idx),
                            verts.at(v2_idx),
                            mat
                        );
                    }
                    
                    object->add(tri);
                }
                objects->add(object);
            }

            return objects;
        }
    private:
        std::vector<vec3> verts, norms, texs;
        std::deque<face_group> face_groups;

        void process_face(const std::vector<std::string>& clusters, std::vector<face_vertex>& vertices) {
            for (auto& index_cluster : clusters) {
                face_vertex v;
                std::istringstream s(index_cluster);
                std::string index;

                // Get the vertex coordinate index
                if (std::getline(s, index, '/') && !index.empty()) {
                    v.vertex_index = std::stoi(index) - 1;
                }

                // Get the texture coordinate index
                if (std::getline(s, index, '/') && !index.empty()) {
                    v.uv_index = std::stoi(index) - 1;
                }

                // Get the normal index
                if (std::getline(s, index, '/') && !index.empty()) {
                    v.normal_index = std::stoi(index) - 1;
                }

                vertices.push_back(v);
            }
        }

};

#endif