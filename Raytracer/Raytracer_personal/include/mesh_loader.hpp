#ifndef MESH_LOADER_HPP
#define MESH_LOADER_HPP

#include <fstream>
#include <vector>
#include <string>
#include <memory>

#include "vec3.hpp"
#include "shapes.hpp"

class obj_loader {
    public:
        bool load(const std::string &fn, std::shared_ptr<material> mat) {
            std::ifstream file(fn);
            if (!file.is_open()) {
                std::cerr << "Failed to open: " << fn << std::endl;
                return false;
            }

            std::string line;
            while (std::getline(file, line)) {
                std::istringstream iss(line);
                std::string prefix;

                iss >> prefix;

                if (prefix == "v") {
                    vec3 vertex;
                    iss >> vertex.e[0] >> vertex.e[1] >> vertex.e[2];
                    vertices.push_back(vertex);
                } else if (prefix == "f") {
                    double v0, v1, v2;
                    // char slash;
                    // iss >> v0 >> slash >> v1 >> slash >> v2;
                    iss >> v0 >> v1 >> v2;
                    faces.push_back(std::make_shared<triangle>(
                        vertices[v0-1], vertices[v1-1], vertices[v2-1], mat));
                }
            }

            file.close();
            return true;
        }

        std::vector<vec3> get_vertices() { return vertices; }

        std::vector<std::shared_ptr<triangle>> get_triangles() { return faces; }

    private:
        std::vector<vec3> vertices;
        std::vector<std::shared_ptr<triangle>> faces;
};

#endif