#include <cstdio>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdint>

#include "game.h"

#include "game.h"

const char *win_name = "App-DEV";

GLFWwindow *root_win;
uint32_t WINDOW_W = 1920, WINDOW_H = 1200;
uint32_t capture_mode = GLFW_CURSOR_NORMAL;

game game(WINDOW_W, WINDOW_H);

void toggle_win_input_mode( GLFWwindow *win ) {
    if ( capture_mode == GLFW_CURSOR_NORMAL ) {
        glfwSetInputMode( win, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
        capture_mode = GLFW_CURSOR_DISABLED;
    }
    else if ( capture_mode == GLFW_CURSOR_DISABLED ) {
        glfwSetInputMode( win, GLFW_CURSOR, GLFW_CURSOR_NORMAL );
        capture_mode = GLFW_CURSOR_NORMAL;
    }
}

void key_callback(GLFWwindow *win, int key, int scancode, int action, int mode) {
    if ( key == GLFW_KEY_LEFT_ALT && action == GLFW_PRESS ) toggle_win_input_mode( win );

    if ( capture_mode == GLFW_CURSOR_NORMAL ) return;

    if ( key >= 0 && key < 1024) {

        if ( action == GLFW_PRESS) {
            // Window related actions
            game.m_keys[key] = true;
        }
        else if ( action == GLFW_RELEASE) {
            game.m_keys[key] = false;
        }
    }
}

void mouse_callback( GLFWwindow *win, double xpos, double ypos ) {
    if ( capture_mode == GLFW_CURSOR_NORMAL ) return;
    game.mouse_pos_callback( xpos, ypos );
}

void scroll_callback( GLFWwindow *window, double xoffset, double yoffset) {
    if ( capture_mode == GLFW_CURSOR_NORMAL ) return;
    game.mouse_scroll_callback( xoffset, yoffset );
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport( 0, 0, width, height );
    game.win_resolution_chng_callback(width, height);
}

int main() {
    glfwInit();
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

    
    root_win = glfwCreateWindow( WINDOW_W, WINDOW_H, win_name, NULL, NULL );

    glfwMakeContextCurrent( root_win );
    glfwSwapInterval( 0 );
    glfwSetInputMode( root_win, GLFW_CURSOR, capture_mode );

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }

    glViewport(0, 0, WINDOW_W, WINDOW_H);
    glfwSetFramebufferSizeCallback(root_win, framebuffer_size_callback);
    glfwSetKeyCallback(root_win, key_callback);
    glfwSetCursorPosCallback( root_win, mouse_callback );
    glfwSetScrollCallback( root_win, scroll_callback );

    glEnable(GL_DEPTH_TEST);

    game.init();


    double lf = glfwGetTime();
    unsigned int f = 0;

    while(game.is_running()) {
        glfwPollEvents();

        float cf = glfwGetTime();
        float dt = cf - lf;
        lf = cf;

        game.read_input(dt);
        game.update(dt);
        game.render(dt);

        glfwSwapBuffers(root_win);
    }

    glfwTerminate();
    return 0;
};
