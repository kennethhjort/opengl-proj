
#ifndef LNX_TRANSFORM_COMP_HPP
#define LNX_TRANSFORM_COMP_HPP

#include <glm/glm.hpp>

struct lnx_transform_comp {
    glm::mat4 model = glm::mat4( 1.0f );
    glm::vec3 position = glm::vec3( 0.0f, 0.0f, 0.0f );
    glm::vec3 rotation = glm::vec3( 0.0f, 0.0f, 0.0f );
    glm::vec3 scale = glm::vec3( 0.0f, 0.0f, 0.0f );
};

#endif
