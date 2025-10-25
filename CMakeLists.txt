cmake_minimum_required(VERSION 3.15)
project(webgpu-compute LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}" CACHE PATH "Install path" FORCE)

include(FetchContent)

# === Fetch precompiled WebGPU (Dawn distribution) ===
FetchContent_Declare(
    webgpu
    URL https://github.com/eliemichel/WebGPU-distribution/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(webgpu)

# === Library ===
add_subdirectory(src)


# === Samples ===
option(BUILD_SAMPLES "Build sample binaries" ON)
if(BUILD_SAMPLES)
  add_subdirectory(samples)
endif()
