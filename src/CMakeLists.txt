add_library(webgpu_compute STATIC
    device_manager.cpp
    buffer_manager.cpp
    compute_engine.cpp
)

target_include_directories(webgpu_compute
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

# Link to the WebGPU dependency fetched above
target_link_libraries(webgpu_compute PUBLIC webgpu)
