
#include <cstdio>
#include <cstring>
#include <glad/glad.h>
#include <stb_image.h>

#include "model_data.hpp"
#include "assimp/types.h"

#include <iostream>

lnx_model_data::lnx_model_data( const char *path ) {
    load_model( path );
}

void lnx_model_data::load_model( std::string const &path ) {
    printf( "load_model\n" );
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile( path, aiProcess_Triangulate | aiProcess_FlipUVs );

    if ( !scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode ) {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    directory = path.substr( 0, path.find_last_of( '/' ) );

    process_node( scene->mRootNode, scene );
}

void lnx_model_data::process_node( aiNode *node, const aiScene *scene ) {
    printf( "process_node\n" );
    for ( unsigned int i = 0; i< node->mNumMeshes; i++ ) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back( process_mesh( mesh, scene ) );
    }

    for ( unsigned int i = 0; i < node->mNumChildren; i++ ) {
        process_node( node->mChildren[i], scene );
    }
}

lnx_mesh_data lnx_model_data::process_mesh( aiMesh *mesh, const aiScene *scene ) {
    printf( "process_mesh\n" );

    vector<lnx_vertex> vertices;
    vector<unsigned int> indices;
    vector<lnx_texture> textures;

    for ( unsigned int i = 0; i < mesh->mNumVertices; i++ ) {
        lnx_vertex vertex;

        // Process vertex position, normals and texture coords
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;

        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.normal = vector;

        if ( mesh->mTextureCoords[0] ) {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.uv = vec;
        }
        else vertex.uv = glm::vec2( 0.0f, 0.0f );

        vertices.push_back( vertex );
    }

    // Process indices
    for ( unsigned int i = 0; i < mesh->mNumFaces; i++ ) {
        aiFace face = mesh->mFaces[i];
        for ( unsigned int j = 0; j < face.mNumIndices; j++ ) {
            indices.push_back( face.mIndices[j] );
        }
    }

    // Process material
    if ( mesh->mMaterialIndex >= 0 ) {
        aiMaterial *material = scene->mMaterials[ mesh->mMaterialIndex ];
        vector<lnx_texture> dif_maps = load_material_textures( material, aiTextureType_DIFFUSE, "texture_diffuse" );
        textures.insert( textures.end(), dif_maps.begin(), dif_maps.end() );

        vector<lnx_texture> spec_maps = load_material_textures( material, aiTextureType_SPECULAR, "texture_specular" );
        textures.insert( textures.end(), spec_maps.begin(), spec_maps.end() );
    }

    return lnx_mesh_data( vertices, indices, textures );
}

std::vector<lnx_texture> lnx_model_data::load_material_textures( aiMaterial *mat, aiTextureType type, std::string type_name ) {
    printf( "load_material_textures\n" );
    vector<lnx_texture> textures;

    for ( unsigned int i = 0; i < mat->GetTextureCount(type); i++ ) {
        aiString str;
        mat->GetTexture( type, i, &str );
        bool skip = false;
        for ( unsigned int j = 0; j < textures_loaded.size(); j++ ) {
            if ( std::strcmp( textures_loaded[j].path.data(), str.C_Str() ) == 0 ) {
                textures.push_back( textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if ( !skip ) {
            lnx_texture texture;
            texture.id = texture_from_file( str.C_Str(), directory );
            texture.type = type_name;
            texture.path = str.C_Str();
            textures.push_back( texture );
        }
    }

    return textures;
}

unsigned int lnx_model_data::texture_from_file(const char *path, const std::string &directory ) {
    printf( "texture_from_file\n" );
    std::string filename = std::string( path );
    filename = directory + '/' + filename;

    unsigned int tex_id;
    glGenTextures( 1, &tex_id );

    int width, height, nr_cmpn;
    stbi_set_flip_vertically_on_load(false);
    unsigned char *data = stbi_load( filename.c_str(), &width, &height, &nr_cmpn, 0 );

    if ( data ) {
        GLenum format;
        if ( nr_cmpn == 1 ) format = GL_RED;
        if ( nr_cmpn == 3 ) format = GL_RGB;
        if ( nr_cmpn == 4 ) format = GL_RGBA;

        glBindTexture( GL_TEXTURE_2D, tex_id );
        glTexImage2D( GL_TEXTURE_2D,  0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data );
        glGenerateMipmap( GL_TEXTURE_2D );

        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

        stbi_image_free( data );
    }
    else {
        std::cout << "MODEL::TEXTURE: Texture failed to load at path : " << path << std::endl;
    }
    return tex_id;
}
