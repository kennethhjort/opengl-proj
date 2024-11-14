
#pragma once

#include <glm/glm.hpp>
#include "types.hpp"

class shader_manager {
    
    public:
        void compile( lnx_shader &shader, const char *vPath, const char *fPath ) const;

        void set_int( lnx_shader &shader, const char *name, int value ) const;
        void set_mat4( lnx_shader &shader, const char *name, glm::mat4 &matrix ) const;
        
    private:
        typedef enum {
            PROGRAM,
            VERTEX,
            FRAGMENT,
            GEOMETRY
        } COMPILE_TYPES;

        void check_compile_error( unsigned int object, unsigned int type ) const;
};
