
#include "resource_manager.hpp"

#include <glm/glm.hpp>
#include <vector>
#include "glm/gtc/matrix_transform.hpp"
#include "types.hpp"

lnx_camera_comp ResourceManager::camera_init( float fov, float scr_width, float scr_height, float near_plane, float far_plane ) {
    lnx_camera_comp camera;

    camera.projection = glm::perspective( glm::radians( fov ), scr_width / scr_height, near_plane, far_plane );
    camera.view = glm::translate( camera.view, glm::vec3( camera.position ) );
    
    // Camera Up vector ( Unanitialized )
    glm::vec3 dir = glm::vec3( glm::normalize( camera.position - glm::vec3( 0.0f, 0.0f, 0.0f ) ) );
    glm::vec3 right = glm::normalize( glm::cross( glm::vec3 ( 0.0f, 1.0f, 0.0f ), dir ) );
    camera.up = glm::cross( dir, right );
    
    return camera;
}

void ResourceManager::camera_update_projection( lnx_camera_comp &camera, float scr_width, float scr_height ) {
    camera.projection = glm::perspective( 
            glm::radians( camera.fov ), 
            scr_width / scr_height, 
            camera.near_plane, 
            camera.far_plane );
}

lnx_model ResourceManager::model_init( const char *path ) {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;

    m_objloader.load_obj( path, vertices, uvs, normals );
    
}
