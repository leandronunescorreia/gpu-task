add_executable(vector_add main.cpp)

target_include_directories(vector_add PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(vector_add PRIVATE webgpu_compute)
