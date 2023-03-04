#include "loader.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <iostream>
#include <vector>

namespace c3po {

bool load_obj(const char* filename) {

    std::cout << "Loading " << filename << std::endl;

    tinyobj::ObjReader reader;
    if (!reader.ParseFromFile(filename)) {
        if (!reader.Error().empty()) {
            std::cerr << "TinyObjReader: " << reader.Error();
        }
        exit(1);
    }

    if (!reader.Warning().empty()) {
        std::cout << "TinyObjReader: " << reader.Warning();
    }

    tinyobj::attrib_t attrib = reader.GetAttrib();
    std::vector<tinyobj::shape_t> shapes = reader.GetShapes();
    std::vector<tinyobj::material_t> materials = reader.GetMaterials();

    return true;
}

} // namespace c3po
