#pragma once
#include <filesystem>
#include <string>
#include <fstream>
#include <format>

namespace fs = std::filesystem;

namespace omgl {

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

}