
#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstdint>
#include <cstdio>
#include <vector>

#include "engine/core/components/camera.hpp"
#include "engine/core/components/renderable.hpp"
#include "engine/core/components/transform.hpp"
#include "engine/obj/model_data.hpp"
#include "engine/render_system.hpp"
#include "engine/resource_manager.hpp"
#include "engine/shader_manager.hpp"
#include "engine/texture_manager.hpp"
#include "engine/types.hpp"

enum game_state {
    GAME_ACTIVE,
    GAME_MENU,
};

class game {
    public:
        bool m_keys[1024];

        game(uint32_t w, uint32_t h) : m_state(GAME_ACTIVE), m_width(w), m_height(h), mouse_last_x(w / 2.0f), mouse_last_y(h / 2.0f) { }
        ~game() { }

        bool is_running() { 
            return m_is_running; 
        }

        void mouse_pos_callback(double xpos, double ypos) {
            float x = static_cast<float>(xpos);
            float y = static_cast<float>(ypos);

            if ( mouse_first ) {
                mouse_last_x = x;
                mouse_last_y = y;
                mouse_first = false;
            }

            float xoffset = x - mouse_last_x;
            float yoffset = mouse_last_y - y;
            mouse_last_x = x;
            mouse_last_y = y;

            const float sensitivity = 0.08f;
            
            player_camera.yaw += xoffset * sensitivity;
            player_camera.pitch -= yoffset * sensitivity;

            if ( player_camera.pitch > 89.0f ) player_camera.pitch = 89.0f;
            if ( player_camera.pitch < -89.0f ) player_camera.pitch = -89.0f;

            player_camera.position.x = m_transforms[0].position.x + 3.0f * cos( glm::radians( player_camera.pitch ) ) * cos( glm::radians( player_camera.yaw ) );
            player_camera.position.y = m_transforms[0].position.y + 3.0f * sin( glm::radians( player_camera.pitch ) );
            player_camera.position.z = m_transforms[0].position.z + 3.0f * cos( glm::radians( player_camera.pitch ) ) * sin( glm::radians( player_camera.yaw ) );
        }

        void mouse_scroll_callback(double xoffset, double yoffset) {
            player_camera.fov -= yoffset;
            player_camera.projection = glm::perspective( glm::radians( player_camera.fov ), m_width / m_height, 1.0f, 100.0f );
        }

        void win_resolution_chng_callback(float w, float h) {
            m_width = w;
            m_height = h;
            player_camera.projection = glm::perspective( glm::radians( player_camera.fov), m_width / m_height, 0.1f, 100.0f );
            printf( "Width: %f, Height: %f\n", w, h );
        }

        void read_input(float dt) {
            // Game Active State keybinds
            if ( this->m_state == GAME_ACTIVE ) {
                
                // Should not run if escape
                if ( this->m_keys[GLFW_KEY_ESCAPE] ) this->m_is_running = false;
                
                // Wireframe and back
                // TODO: Make switch instead on single button
                if ( this->m_keys[GLFW_KEY_1] ) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                if ( this->m_keys[GLFW_KEY_2] ) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

                // Camera Movement
                const float camera_speed = 5.0f * dt;
                glm::vec3 dir( 0.0f );

                if ( this->m_keys[GLFW_KEY_W] ) {
                    dir.z += camera_speed;
                    // player_camera[0].position += camera_speed * m_cameras[0].front;
                }
                if ( this->m_keys[GLFW_KEY_S] ) {
                    dir.z -= camera_speed;
                    // player_camera[0].position -= camera_speed * m_cameras[0].front;
                }
                if ( this->m_keys[GLFW_KEY_A] ) {
                    dir.x -= camera_speed;
                    // player_camera[0].position -= glm::normalize( glm::cross( m_cameras[0].front, m_cameras[0].up ) ) * camera_speed;
                }
                if ( this->m_keys[GLFW_KEY_D] ) {
                    dir.x += camera_speed;
                    // player_camera[0].position += glm::normalize( glm::cross( m_cameras[0].front, m_cameras[0].up ) ) * camera_speed;
                }
                move_player( dir );

            }
            
            else if ( this->m_state == GAME_MENU ) {

            }

        }


        void move_player( glm::vec3 dir ) {
            glm::vec3 f = glm::normalize( glm::vec3( m_transforms[0].position.x - player_camera.position.x, 0.0f, m_transforms[0].position.z - player_camera.position.z ) );
            glm::vec3 r = glm::normalize( glm::cross( f, player_camera.up ) );

            m_transforms[0].position += dir.z * f;
            m_transforms[0].position += dir.x * r;


            player_camera.position += dir.z * f;
            player_camera.position += dir.x * r;
        }

        void init() {
            // Camera setup
            player_camera = m_res_man.camera_init( 65.0f, m_width, m_height, 0.1f, 100.0f );

            // Shader Compilation
            m_shader_manager.compile( m_shader, "shader/shader.vs", "shader/shader.fs");

            // Model Initialization
            lnx_model_data player_model( "models/player/mr_bean.obj" );
            lnx_renderable_comp player_comp = { .shader = m_shader, .model = player_model };
            m_renderables.push_back( player_comp );
            lnx_transform_comp player_transform;
            m_transforms.push_back( player_transform );

            lnx_model_data ground_model( "models/world/world.obj" );
            lnx_renderable_comp ground_comp = { .shader = m_shader, .model = ground_model };
            m_renderables.push_back( ground_comp );
            lnx_transform_comp world_transform;
            m_transforms.push_back( world_transform );

            lnx_model_data create_model( "models/house/house.obj" );
            lnx_renderable_comp create_comp = { .shader = m_shader, .model = create_model };
            m_renderables.push_back( create_comp );
            lnx_transform_comp create_transform;
            m_transforms.push_back( create_transform );
        }

        void update(double dt) {
            glm::mat4 player_mod = glm::mat4( 1.0f );
            m_transforms[0].model = glm::translate( player_mod, m_transforms[0].position );

            player_camera.view = glm::lookAt( player_camera.position, m_transforms[0].position, player_camera.up );
        }

        void render(double dt) {
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            m_render.render_models( m_renderables, m_transforms, player_camera );
        }

    private:

        Render_System m_render;

        std::vector<lnx_renderable_comp> m_renderables;
        std::vector<lnx_transform_comp> m_transforms;
        lnx_camera_comp player_camera;

        lnx_shader m_shader;
        shader_manager m_shader_manager;
        texture_manager m_tex_manager;
        ResourceManager m_res_man;

        bool mouse_first = true;
        float mouse_last_x, mouse_last_y;

        game_state m_state;
        float m_width;
        float m_height;
        int *m_win;
        bool m_is_running = true;

};
