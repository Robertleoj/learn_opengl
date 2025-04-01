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
const fs::path orange_frag_path = base / "shaders" / "just_orange.frag";
const fs::path blue_frag_path = base / "shaders" / "just_blue.frag";

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

gl::GLuint set_array_buffer(
    const std::vector<float>& data
) {
    gl::GLuint vertex_buffer_id;
    gl::glGenBuffers(1, &vertex_buffer_id);

    spdlog::info("vbo id: {}", vertex_buffer_id);

    gl::glBindBuffer(gl::GL_ARRAY_BUFFER, vertex_buffer_id);

    gl::glBufferData(
        gl::GL_ARRAY_BUFFER,
        data.size() * sizeof(float),
        data.data(),
        gl::GL_STATIC_DRAW
    );

    return vertex_buffer_id;
}

gl::GLuint set_element_buffer(
    const std::vector<gl::GLuint>& indices
) {
    gl::GLuint element_array_buffer_id;
    gl::glGenBuffers(1, &element_array_buffer_id);

    gl::glBindBuffer(gl::GL_ELEMENT_ARRAY_BUFFER, element_array_buffer_id);
    gl::glBufferData(
        gl::GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(gl::GLuint),
        indices.data(),
        gl::GL_STATIC_DRAW
    );

    return element_array_buffer_id;
}

class Square {
   public:
    Square(
        std::vector<float> vertices
    )
        : vertices(vertices) {
        this->indices = {

            // top right triangle
            0,
            1,
            2,
            // bottom left triangle
            0,
            2,
            3
        };
    }

    void init() {
        gl::glGenVertexArrays(1, &this->vao_id);

        gl::glBindVertexArray(this->vao_id);

        auto vertex_buffer_id = set_array_buffer(this->vertices);
        auto element_buffer_id = set_element_buffer(this->indices);

        gl::glVertexAttribPointer(
            0,
            3,
            gl::GL_FLOAT,
            gl::GL_FALSE,
            3 * sizeof(float),
            static_cast<void*>(0)
        );
        gl::glEnableVertexAttribArray(0);

        gl::glBindVertexArray(0);
    }

    void draw() {
        gl::glBindVertexArray(this->vao_id);
        gl::glDrawElements(gl::GL_TRIANGLES, 6, gl::GL_UNSIGNED_INT, 0);
        gl::glBindVertexArray(0);
    }

   private:
    gl::GLuint vao_id;
    std::vector<float> vertices;
    std::vector<gl::GLuint> indices;
};

class Triangle {
   public:
    Triangle(
        std::vector<float> vertices
    )
        : vertices(vertices) {}

    void init() {
        gl::glGenVertexArrays(1, &this->vao_id);

        gl::glBindVertexArray(this->vao_id);

        auto vertex_buffer_id = set_array_buffer(this->vertices);

        gl::glVertexAttribPointer(
            0,
            3,
            gl::GL_FLOAT,
            gl::GL_FALSE,
            3 * sizeof(float),
            static_cast<void*>(0)
        );
        gl::glEnableVertexAttribArray(0);

        gl::glBindVertexArray(0);
    }

    void draw() {
        gl::glBindVertexArray(this->vao_id);
        gl::glDrawArrays(gl::GL_TRIANGLES, 0, 3);
        gl::glBindVertexArray(0);
    }

   private:
    gl::GLuint vao_id;
    std::vector<float> vertices;
};

int main() {
    spdlog::set_level(spdlog::level::debug);

    const std::size_t window_width = 800, window_height = 600;

    auto window =
        omgl::make_window("Hello triangle", window_width, window_height);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    auto vertex_shader_id = omgl::compile_vertex_shader(vertex_shader_path);
    auto blue_frag_id = omgl::compile_fragment_shader(blue_frag_path);
    auto orange_frag_id = omgl::compile_fragment_shader(orange_frag_path);

    auto orange_shader_program_id =
        omgl::make_shader_program(vertex_shader_id, orange_frag_id);

    auto blue_shader_program_id =
        omgl::make_shader_program(vertex_shader_id, blue_frag_id);

    Square square1({

        // top left
        -0.5f,
        0.5f,
        0.0f,
        // top right
        0.0f,
        0.5f,
        0.0f,
        // bottom right
        0.0f,
        0.0f,
        0.0f,
        // bottom left
        -0.5f,
        0.0f,
        0.0f
    });
    square1.init();

    Square square2({

        // top left
        0.0f,
        0.0f,
        0.0f,
        // top right
        0.5f,
        0.0f,
        0.0f,
        // bottom right
        0.5f,
        -0.5f,
        0.0f,
        // bottom left
        0.0f,
        -0.5f,
        0.0f
    });
    square2.init();

    Triangle triangle({

        // v1
        0.0f,
        0.5f,
        0.0f,
        // v2
        0.5f,
        0.5f,
        0.0f,
        // v3
        0.5f,
        0.0f,
        0.0f
    });
    triangle.init();

    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        gl::glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        gl::glClear(gl::GL_COLOR_BUFFER_BIT);

        gl::glUseProgram(blue_shader_program_id);

        square1.draw();
        square2.draw();

        gl::glUseProgram(orange_shader_program_id);
        triangle.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}