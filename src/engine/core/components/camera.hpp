
#ifndef LNX_CAMERA_COMP_HPP
#define LNX_CAMERA_COMP_HPP

#include <glm/glm.hpp>

struct lnx_camera_comp {
    // For Rendering objects in cameras view matrix
    glm::mat4 projection = glm::mat4( 1.0f );
    glm::mat4 view = glm::mat4( 1.0f );
    float near_plane = 0.1f;
    float far_plane = 100.0f;

    // For cameras position and direction
    glm::vec3 position = glm::vec3( 0.0f, 0.0f, 3.0f ); // Initial camera position
    glm::vec3 up;
    glm::vec3 front = glm::vec3( 0.0f, 0.0f, -1.0f );

    // For mouse movement calculations
    float pitch = 0.0f, yaw = -90.0f;
    float fov = 65.0f;
};

#endif
