// glfw is including some opengl helper functions
// but glbinding does that as well, so we don't want that
// This constant tells glfw not to include those
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
#include <spdlog/spdlog.h>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <omgl/glfw.hpp>
#include <omgl/shaders.hpp>

namespace fs = std::filesystem;

const fs::path base = fs::path(__FILE__).parent_path();
const fs::path vertex_shader_path = base / "shaders" / "vertex_shader.vert";
const fs::path fragment_shader_path = base / "shaders" / "fragment_shader.frag";

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

    const std::size_t window_width = 800;
    const std::size_t window_height = 600;

    auto window =
        omgl::make_window("Hello triangle", window_width, window_height);

    glfwMakeContextCurrent(window);

    glbinding::initialize(glfwGetProcAddress);

    gl::glViewport(0, 0, window_width, window_height);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    std::array<float, 3 * 3> arr = {
        -0.5f,
        -0.5f,
        0.0f,
        //
        0.5f,
        -0.5f,
        0.0f,
        //
        0.0f,
        0.5f,
        0.0f
    };

    // create a vertex buffer object
    // state-setting
    gl::GLuint vbo;
    // this is just a buffer object - we are creating/declaring a buffer
    gl::glGenBuffers(1, &vbo);

    spdlog::info("vbo id: {}", vbo);

    // bind the buffer to the gl::GL_ARRAY_BUFFER target
    // state-setting
    // the GL_ARRAY_BUFFER is probably some slot that OpenGL has
    // and we want to bind the buffer to it
    gl::glBindBuffer(
        // we are binding to the gl array buffer target
        gl::GL_ARRAY_BUFFER,
        // the object we are binding to the target
        vbo
    );

    // copy the data into the buffer's memory
    // state-setting

    // we now have a buffer that is bound to a target
    // but we don't have any data in the buffer

    // question: why don't we copy into the bufer using the vbo variable?
    // why do we specify the target instead of the vertex buffer object ID?
    gl::glBufferData(
        // copy to the buffer bound to this target
        gl::GL_ARRAY_BUFFER,
        // the number of bytes to copy
        sizeof(arr),
        // pointer to the data
        arr.data(),
        // This means we will only set the data once, and use it many times
        // we could use GL_STREAM_DRAW if we were going to use the data only a
        // few times or we could use GL_DYNAMIC_DRAW if we were going to change
        // the data a lot
        gl::GL_STATIC_DRAW
    );

    auto vertex_shader_id = omgl::compile_vertex_shader(vertex_shader_path);
    auto fragment_shader_id =
        omgl::compile_fragment_shader(fragment_shader_path);

    gl::GLuint shader_program_id = gl::glCreateProgram();
    gl::glAttachShader(shader_program_id, vertex_shader_id);
    gl::glAttachShader(shader_program_id, fragment_shader_id);
    gl::glLinkProgram(shader_program_id);

    omgl::ensure_shader_program_linked(shader_program_id);

    gl::glDeleteShader(vertex_shader_id);
    gl::glDeleteShader(fragment_shader_id);

    // Before we start setting states to draw our object, we need
    // to create a Vertex Array Object that stores the object state.
    gl::GLuint vertex_array_obj_id;

    // we can create many at a time, but we just create one this time.
    gl::glGenVertexArrays(1, &vertex_array_obj_id);

    // We enable the vertex array object before we start fiddling with state
    gl::glBindVertexArray(vertex_array_obj_id);

    // now we specify how our vertices are laid out in memory
    // and which variables they should be linked to in our shaders
    // Since vbo is still bound to GL_ARRAY_BUFFER, opengl will attach
    // vbo to attribute 0.
    glVertexAttribPointer(
        // This is for attribute 0 (location=0 in our vertex shader)
        0,
        // This is the size of the vertex attribute - it is a vec3, so
        // it is composed of 3 values
        3,
        // The type is floating point (float*)
        gl::GL_FLOAT,
        // This argument specifies whether we want to squish our inputs
        // to a normalized range, usually not needed for floats
        gl::GL_FALSE,
        // This is the stride of the data - what is the distance between
        // pointers to the first values of the vectors
        // We can also pass 0, which tells OpenGL that we have tightly
        // packed values
        3 * sizeof(float),
        // This is a weird argument - it represents the offset to the first
        // value given the pointer to the beginning of the vectors. Our values
        // have 0 offset, so we specify 0 (so what is the void* cast for?)
        static_cast<void*>(0)
    );
    // vertex attributes are disabled by default, so we tell OpenGL to
    // enable the vertex attribute at location=0
    gl::glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        // state-setting - it sets the color used to clear the screen
        gl::glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        // state-using - clears the state setting the color
        // to the color set above
        gl::glClear(gl::GL_COLOR_BUFFER_BIT);

        //
        gl::glUseProgram(shader_program_id);
        gl::glBindVertexArray(vertex_array_obj_id);
        gl::glDrawArrays(gl::GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}