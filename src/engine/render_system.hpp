
#pragma once

#include "core/components/camera.hpp"
#include "core/components/renderable.hpp"
#include "core/components/transform.hpp"
#include "shader_manager.hpp"

#include <vector>

class Render_System {
    public:
        void render_models(std::vector<lnx_renderable_comp> &renderables, std::vector<lnx_transform_comp> &transforms, lnx_camera_comp &camera);

    private:
        shader_manager m_shader_man;

};
