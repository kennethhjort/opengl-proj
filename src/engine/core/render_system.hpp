
#pragma once

#include "components/camera.hpp"
#include "ecs.hpp"

class render_system {
    void update( registry &reg ){
        for ( lnx_transform_comp trans : reg.transform ) {
            trans.position = glm::vec3( 0.0f, 0.0f, 0.0f );
        }
    }
    void render( registry &reg, lnx_camera_comp &cam ) {

    }
};
