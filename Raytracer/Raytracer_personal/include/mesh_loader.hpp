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
                    iss >> v0 >> v1 >> v2;
                    faces.push_back(std::make_shared<triangle>(
                        vertices[v0-1], vertices[v1-1], vertices[v2-1], mat));
                }
            }

            file.close();
            return true;
        }

        bool load_meshes(const std::string &fn, const std::string &mtl_fn) {

            if (!load_mats(mtl_fn)) {
                return false;
            }

            std::ifstream file(fn);
            if (!file.is_open()) {
                std::cerr << "Failed to open: " << fn << std::endl;
                return false;
            }

            std::string line, current_mat;
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
                    iss >> v0 >> v1 >> v2;
                    faces.push_back(std::make_shared<triangle>(
                        vertices[v0-1], vertices[v1-1], vertices[v2-1], mats[current_mat]));
                } else if (prefix == "usemtl") {
                    iss >> current_mat;
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
        std::unordered_map<std::string, std::shared_ptr<material>> mats;

        bool load_mats(const std::string &mtl_fn) {
            std::ifstream file(mtl_fn);
            if (!file.is_open()) {
                std::cerr << "failed to open MTL file: " << mtl_fn << std::endl;
                return false;
            }

            std::string line, current_mat;
            vec3 ka, kd, ks, ke;
            double ni = 1.0;
            double d = 1.0;     // transparency
            double illum = 0;
            double ns = 0;

            while (std::getline(file, line)) {
                std::istringstream iss(line);
                std::string prefix;
                iss >> prefix;

                if (prefix == "newmtl") {
                    if (!current_mat.empty()) {
                        // add the last parsed mat
                        mats[current_mat] = create_mat(ka, kd, ks, ke, ni, d, illum, ns);
                    }
                    iss >> current_mat;
                    ka = kd = ks = ke = vec3(0, 0, 0);
                    ni = 1.0;
                    d = 1.0;
                    illum = 0;
                } else if (prefix == "Ka") {
                    iss >> ka.e[0] >> ka.e[1] >> ka.e[2];
                } else if (prefix == "Kd") {
                    iss >> kd.e[0] >> kd.e[1] >> kd.e[2];
                } else if (prefix == "Ks") {
                    iss >> ks.e[0] >> ks.e[1] >> ks.e[2];
                } else if (prefix == "Ke") {
                    iss >> ke.e[0] >> ke.e[1] >> ke.e[2];
                } else if (prefix == "Ni") {
                    iss >> ni;
                } else if (prefix == "d") {
                    iss >> d;
                } else if (prefix == "illum") {
                    iss >> illum;
                } else if (prefix == "Ns") {
                    iss >> ns;
                }
            }

            if (!current_mat.empty()) {
                mats[current_mat] = create_mat(ka, kd, ks, ke, ni, d, illum, ns);
            }

            file.close();
            return true;
        }

        std::shared_ptr<material> create_mat(
            const vec3 &ka, const vec3 &kd, const vec3 &ks, const vec3 &ke, 
            double ni, double d, int illum, double ns) 
        {
            
            if (ke.e[0] > 0 || ke.e[1] > 0 || ke.e[2] > 0) {
                // emissive material (lights)
                return std::make_shared<diffuse_light>(ke);
            }

            if (ni > 1.0) {
                // Dialectric material (glass)
                std::clog << "\nmaking glass material" << std::flush;
                return std::make_shared<dialectric>(ni);
            }

            if (ks.e[0] > 0 || ks.e[1] > 0 || ks.e[2] > 0) {
                // Metalic material (metal)
                double fuzz = 1.0 / (1.0 + std::sqrt(ns));
                std::clog << "\nmaking metal material with a fuzz of: " << fuzz << " and vec3 of: " << kd.x() << " " << kd.y() << " " << kd.z() << std::flush;
                return std::make_shared<metal>(kd, 0.1);
            }

            // default make lambertian material
            std::clog << "\nmaking lambertian material" << std::flush;
            return std::make_shared<lamber>(kd);
        }
};

#endif