
#ifndef LNX_RESOURCE_MANAGER_HPP
#define LNX_RESOURCE_MANAGER_HPP

#include "common/objloader.hpp"
#include "core/components/camera.hpp"
#include "types.hpp"

class ResourceManager {
    public:
        // Camera Related Methods
        lnx_camera_comp camera_init( float fov, float scr_width, float scr_height, float near_plane, float far_plane );
        void camera_update_projection( lnx_camera_comp &camera, float scr_width, float scr_height );

        // Model Related
        lnx_model model_init( const char *path );

    private:
        objloader m_objloader;
};

#endif
