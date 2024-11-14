
#include "render_system.hpp"

#include <GLFW/glfw3.h>

#include "glm/gtc/matrix_transform.hpp"
#include "obj/mesh_data.hpp"
#include "types.hpp"
#include <cstdint>
#include <string>


void Render_System::render_models(std::vector<lnx_renderable_comp> &renderables, std::vector<lnx_transform_comp> &transforms, lnx_camera_comp &camera) {
    for ( unsigned int i = 0; i < renderables.size(); i++ ) {
        glUseProgram( renderables[i].shader.id );

        m_shader_man.set_mat4( renderables[i].shader, "projection", camera.projection );
        m_shader_man.set_mat4( renderables[i].shader, "view", camera.view );


        glm::mat4 model = glm::mat4( 1.0f );
        model = glm::translate( model, transforms[i].position );
        m_shader_man.set_mat4( renderables[i].shader, "model", transforms[i].model );

        for ( lnx_mesh_data mesh : renderables[i].model.meshes ) {
            uint32_t texture_count = 1;
            uint32_t diff_nr = 0;
            uint32_t spec_nr = 0;

            for ( lnx_texture texture : mesh.textures ) {
                glActiveTexture( texture.id );

                std::string nr;
                std::string name = texture.type;

                if ( name == "texture_diffuse" ) {
                    nr = std::to_string( diff_nr++ );
                } else if ( name == "texture_specular" ) {
                    nr = std::to_string( spec_nr++ );
                }

                m_shader_man.set_int(renderables[i].shader, ("material." + name + nr).c_str(), texture_count);
                glBindTexture( GL_TEXTURE_2D, texture.id );

                texture_count++;
            }

            glActiveTexture( GL_TEXTURE0 );

            glBindVertexArray( mesh.vao );
            glDrawElements( GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0 );
            glBindVertexArray( 0 );
        }
    }
}
