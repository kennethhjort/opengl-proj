
#include "shader_manager.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <fstream>
#include <ostream>
#include <sstream>
#include <iostream>

void shader_manager::compile( lnx_shader &shader, const char *vPath, const char *fPath ) const {
    std::string vcode, fcode;
    std::ifstream vsf, fsf;

    vsf.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
    fsf.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
    
    try {
        vsf.open(vPath);
        fsf.open(fPath);

        std::stringstream vss, fss;
        vss << vsf.rdbuf();
        fss << fsf.rdbuf();

        vsf.close();
        fsf.close();

        vcode = vss.str();
        fcode = fss.str();
    }
    catch ( std::ifstream::failure e ) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    
    const char *vsc = vcode.c_str();
    const char *fsc = fcode.c_str();

    // Compile Shaders
    unsigned int v, f;

    v = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(v, 1, &vsc, NULL);
    glCompileShader(v);
    check_compile_error( v, VERTEX );

    f = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(f, 1, &fsc, NULL);
    glCompileShader(f);
    check_compile_error( f, FRAGMENT );

    shader.id = glCreateProgram();
    glAttachShader( shader.id, v );
    glAttachShader( shader.id, f );
    glLinkProgram( shader.id );
    check_compile_error( shader.id, PROGRAM );

    glDeleteShader(v);
    glDeleteShader(f);    
}

void shader_manager::set_mat4( lnx_shader &shader, const char *name, glm::mat4 &matrix ) const {
    glUniformMatrix4fv( glGetUniformLocation( shader.id, name ), 1, false, glm::value_ptr( matrix ) );
}

void shader_manager::set_int( lnx_shader &shader, const char *name, int value ) const {
    glUniform1i( glGetUniformLocation( shader.id, name ), value );
};

void shader_manager::check_compile_error( unsigned int object, unsigned int type ) const {
    int success;
    char infLog[1024];

    switch ( (COMPILE_TYPES)type ) {
        case PROGRAM:
            glGetProgramiv( object, GL_LINK_STATUS, &success );
            if ( !success ) {
                glGetProgramInfoLog( object, 1024, NULL, infLog );
                std::cout << "ERROR::PROGRAM: Link-time error: \n" << infLog << std::endl;
            }
            break;
        case VERTEX:
        case FRAGMENT:
        case GEOMETRY:
            glGetShaderiv( object, GL_COMPILE_STATUS, &success );
            if ( !success ) {
                glGetShaderInfoLog( object, 1024, NULL, infLog );
                std::cout << "ERROR::SHADER: Compile-time error: \n" << infLog << std::endl;
            }
            break;
        default:
            std::cout << "ERROR::SHADER: Unknown shader type: \n" << std::endl;
            break;
    }
}
