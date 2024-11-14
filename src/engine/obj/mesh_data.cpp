

#include "mesh_data.hpp"

#include <glad/glad.h>

lnx_mesh_data::lnx_mesh_data( vector<lnx_vertex> vertices, vector<unsigned int> indices, vector<lnx_texture> textures ) {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    setup_mesh();
}

void lnx_mesh_data::setup_mesh() {
    glGenVertexArrays( 1, &this->vao );
    glGenBuffers( 1, &this->vbo );
    glGenBuffers( 1, &this->ebo );

    glBindVertexArray( this->vao );
    glBindBuffer( GL_ARRAY_BUFFER, this->vbo );

    glBufferData( GL_ARRAY_BUFFER, vertices.size() * sizeof(lnx_vertex), &vertices[0], GL_STATIC_DRAW );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, this->ebo );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW );

    // Vertex Position
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof(lnx_vertex), (void *) 0 );
    glEnableVertexAttribArray( 0 );

    // Vertex Normals
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof(lnx_vertex), (void*)offsetof(lnx_vertex, normal) );
    glEnableVertexAttribArray( 1 );

    glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, sizeof(lnx_vertex), (void*)offsetof(lnx_vertex, uv) );
    glEnableVertexAttribArray( 2 );

    glBindVertexArray( 0 );
}
