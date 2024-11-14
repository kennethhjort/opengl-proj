
#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>

struct lnx_vertex {
    glm::vec3 position;
    glm::vec2 uv;
    glm::vec3 normal;
};

struct lnx_texture {
    unsigned int id;
    std::string type;
    std::string path;
};

struct lnx_mesh {
    unsigned int id;

    uint32_t indices_size;
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
};

struct lnx_model {
    std::vector<lnx_mesh> meshes;
    std::vector<lnx_texture> textures;
};

struct lnx_shader {
    unsigned int id;
};
