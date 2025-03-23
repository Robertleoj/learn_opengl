// glfw is including some opengl helper functions
// but glbinding does that as well, so we don't want that
// This constant tells glfw not to include those
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
#include <spdlog/spdlog.h>
#include <iostream>

void framebuffer_size_callback(
    GLFWwindow* window,
    int width,
    int height
) {
    spdlog::debug("Window resized");
    gl::glViewport(0, 0, width, height);
}

int main() {
    spdlog::set_level(spdlog::level::debug);
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window =
        glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);

    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glbinding::initialize(glfwGetProcAddress);

    gl::glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    while (!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}