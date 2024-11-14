
#pragma once

#include "../types.hpp"
#include "mesh_data.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>

class lnx_model_data {
    public:
        vector<lnx_mesh_data> meshes;
        vector<lnx_texture> textures_loaded;

        lnx_model_data( const char *path );

        lnx_model get_model();
    private:

        std::string directory;

        void load_model( std::string const &path );
        void process_node( aiNode *node, const aiScene *scene );
        lnx_mesh_data process_mesh( aiMesh *mesh, const aiScene *scene );
        std::vector<lnx_texture> load_material_textures( aiMaterial *mat, aiTextureType type, std::string type_name );


        unsigned int texture_from_file(const char *path, const std::string &directory );
};
