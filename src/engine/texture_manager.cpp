
#include "texture_manager.hpp"

#include <cstdio>
#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>

void texture_manager::gen_texture(lnx_tex2d &tex) { 
    glGenTextures( 1, &tex.id );
    glBindTexture( GL_TEXTURE_2D, tex.id );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST ); // Change to LINEAR
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    unsigned char *dta = stbi_load( tex.texPath, &tex.width, &tex.height, &tex.nrch, 0 );
    if ( dta ) {
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, tex.width, tex.height, 0, GL_RGB, GL_UNSIGNED_BYTE, dta );
        glGenerateMipmap( GL_TEXTURE_2D );

        printf( "Generated Texture\nNrChannels: %i\nWidth: %i\nHeight: %i\nTexPath: %s\n", tex.nrch, tex.width, tex.height, tex.texPath );
    }
    else {
        std::cout << "ERROR::TEXTURE: Failed to load texture" << std::endl;
    } 
    stbi_image_free( dta );
}
