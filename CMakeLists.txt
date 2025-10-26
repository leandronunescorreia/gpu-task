cmake_minimum_required(VERSION 3.15)
project(webgpu-compute LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# === Installation prefix (default to build dir) ===
set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}" CACHE PATH "Install path" FORCE)

include(FetchContent)

# === Ensure third-party folder exists ===
set(THIRD_PARTY_DIR "${CMAKE_SOURCE_DIR}/third-party")
file(MAKE_DIRECTORY "${THIRD_PARTY_DIR}")

# === Define local cache for WebGPU-distribution ===
set(WEBGPU_ZIP "${THIRD_PARTY_DIR}/WebGPU-distribution.zip")

# === Download manually only if not exists ===
if (NOT EXISTS "${WEBGPU_ZIP}")
    message(STATUS "Downloading WebGPU-distribution to ${WEBGPU_ZIP} ...")
    file(DOWNLOAD
        "https://github.com/eliemichel/WebGPU-distribution/archive/refs/heads/main.zip"
        "${WEBGPU_ZIP}"
        SHOW_PROGRESS
        STATUS DOWNLOAD_STATUS
    )
    list(GET DOWNLOAD_STATUS 0 DOWNLOAD_OK)
    if (NOT DOWNLOAD_OK EQUAL 0)
        message(FATAL_ERROR "Failed to download WebGPU-distribution (status: ${DOWNLOAD_STATUS})")
    endif()
else()
    message(STATUS "Using cached WebGPU-distribution at ${WEBGPU_ZIP}")
endif()

# === Standard FetchContent usage ===
FetchContent_Declare(
    webgpu
    URL "file://${WEBGPU_ZIP}"
)
FetchContent_MakeAvailable(webgpu)

# === Library ===
add_subdirectory(src)

# === Samples ===
option(BUILD_SAMPLES "Build sample binaries" ON)
if(BUILD_SAMPLES)
  add_subdirectory(samples)
endif()

# === Tests ===
option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
  add_subdirectory(test)
endif()
