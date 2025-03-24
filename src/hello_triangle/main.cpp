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

namespace fs = std::filesystem;

const fs::path base = fs::path(__FILE__).parent_path();
const fs::path vertex_shader_path = base / "shaders" / "vertex_shader.vert";

std::string read_file_text(
    fs::path path
) {
    std::ifstream file(path);

    if (!file) {
        throw std::runtime_error(std::format("Couldn't open {}", path.string())
        );
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

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

GLFWwindow* get_window() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window =
        glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);

    if (window == nullptr) {
        glfwTerminate();
        throw std::runtime_error("Failed to create window");
    }

    return window;
}

unsigned int compile_vertex_shader(
    fs::path shader_path
) {
    // now we create the vertex shader
    unsigned int vertex_shader;
    vertex_shader = gl::glCreateShader(gl::GL_VERTEX_SHADER);

    // read the shader source
    auto vertex_shader_source = read_file_text(vertex_shader_path);
    const char* vertex_shader_source_cstr = vertex_shader_source.c_str();

    spdlog::info(std::format("Shader source: {}", vertex_shader_source));

    // set the shader's source
    gl::glShaderSource(
        // the shader whose source we are specifying
        vertex_shader,
        // the number of source files we are providing
        1,
        // the actual pointer to the source
        // probably a pointer pointer because we might specify multiple files
        &vertex_shader_source_cstr,
        // we don't need to specify the length, as they are null-terminated
        nullptr
    );
    gl::glCompileShader(vertex_shader);

    int success;
    char info_log_buf[512];

    gl::glGetShaderiv(vertex_shader, gl::GL_COMPILE_STATUS, &success);

    if (!success) {
        gl::glGetShaderInfoLog(vertex_shader, 512, nullptr, info_log_buf);
        std::string info_log(info_log_buf);
        throw std::runtime_error(
            std::format("Couldn't compile shader: {}", info_log_buf)
        );
    }

    return vertex_shader;
}

int main() {
    spdlog::set_level(spdlog::level::debug);
    auto window = get_window();

    glfwMakeContextCurrent(window);

    glbinding::initialize(glfwGetProcAddress);

    gl::glViewport(0, 0, 800, 600);

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
    unsigned int vbo;
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

    auto vertex_shader = compile_vertex_shader(vertex_shader_path);

    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        // state-setting
        gl::glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        // state-using
        gl::glClear(gl::GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}