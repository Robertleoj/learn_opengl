// glfw is including some opengl helper functions
// but glbinding does that as well, so we don't want that
// This constant tells glfw not to include those
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
#include <spdlog/spdlog.h>
#include <filesystem>
#include <iostream>
#include <omgl/glfw.hpp>
#include <omgl/shaders.hpp>

namespace fs = std::filesystem;

const fs::path base = fs::path(__FILE__).parent_path();
const fs::path shaders_dir = base / "shaders";

void framebuffer_size_callback(
    GLFWwindow* window,
    int width,
    int height
) {
    spdlog::debug("Window resized");
    gl::glViewport(0, 0, width, height);
}

void process_input(
    GLFWwindow* window
) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int main() {
    spdlog::set_level(spdlog::level::debug);
    const uint window_height = 1000, window_width = 1000;
    auto window =
        omgl::make_window("hello_shaders", window_width, window_height);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    fs::path triangle_vertex_shader_path = shaders_dir / "triangle_basic.vert";
    fs::path triangle_frag_shader_path = shaders_dir / "triangle_basic.frag";

    auto shader_program = omgl::ShaderProgram(
        triangle_vertex_shader_path, triangle_frag_shader_path
    );

    std::array<float, 3 * 3> triangle_vertices = {

        // v1
        0.0f,
        0.5f,
        0.0f,
        // v2
        0.5f,
        -0.5f,
        0.0f,
        // v3
        -0.5f,
        -0.5f,
        0.0f
    };

    // create the triangel vertex array object
    gl::GLuint triangle_vao_id;
    gl::glGenVertexArrays(1, &triangle_vao_id);

    // activate it
    gl::glBindVertexArray(triangle_vao_id);

    // create the triangle vertex buffer
    gl::GLuint triangle_vertex_buffer_id;
    gl::glGenBuffers(1, &triangle_vertex_buffer_id);

    // bind the triangle vertex buffer to the array buffer target
    gl::glBindBuffer(gl::GL_ARRAY_BUFFER, triangle_vertex_buffer_id);
    // copy the data
    gl::glBufferData(
        gl::GL_ARRAY_BUFFER,
        triangle_vertices.size() * sizeof(float),
        triangle_vertices.data(),
        gl::GL_STATIC_DRAW
    );

    // assign the vertex buffer to the shader vertex attribute
    gl::glVertexAttribPointer(
        0,
        3,
        gl::GL_FLOAT,
        gl::GL_FALSE,
        3 * sizeof(float),
        static_cast<void*>(0)
    );

    // enable the shader vertex attribute
    gl::glEnableVertexAttribArray(0);

    gl::glBindVertexArray(0);

    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        // state-setting
        gl::glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        // state-using
        gl::glClear(gl::GL_COLOR_BUFFER_BIT);

        shader_program.use();
        gl::glBindVertexArray(triangle_vao_id);

        gl::glDrawArrays(gl::GL_TRIANGLES, 0, 3);

        gl::glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}