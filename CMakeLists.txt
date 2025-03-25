cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_COMPILER /usr/bin/gcc-13)
set(CMAKE_CXX_COMPILER /usr/bin/g++-13)
set(CMAKE_LINKER /usr/bin/ldd)

set(VCPKG_TARGET_ARCHITECTURE x64)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE dynamic)

project(project)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed/x64-linux-dynamic")

set(VCPKG_TARGET_TRIPLET "x64-linux" CACHE STRING "")


find_package(glbinding CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_subdirectory(lib)
add_subdirectory(src)
