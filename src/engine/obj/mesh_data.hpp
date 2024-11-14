
#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "../types.hpp"

using std::vector;

class lnx_mesh_data {
    
    public:

        unsigned int vao, vbo, ebo;

        vector<lnx_vertex> vertices;
        vector<unsigned int> indices;
        vector<lnx_texture> textures;

        lnx_mesh_data( vector<lnx_vertex> vertices, vector<unsigned int> indices, vector<lnx_texture> textures );
    private:

        void setup_mesh();
};
