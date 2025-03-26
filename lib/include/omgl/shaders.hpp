#pragma once
#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
#include <filesystem>

namespace fs = std::filesystem;
namespace omgl {

void ensure_shader_compiled(gl::GLuint shader_id);

void ensure_shader_program_linked(gl::GLuint program_id);

gl::GLuint compile_vertex_shader(std::string source);
gl::GLuint compile_vertex_shader(fs::path path);

gl::GLuint compile_fragment_shader(std::string source);
gl::GLuint compile_fragment_shader(fs::path path);

gl::GLuint make_shader_program(
    gl::GLuint vertex_shader_id,
    gl::GLuint fragment_shader_id
);

}  // namespace omgl