add_executable(vector_add main.cpp)

target_link_libraries(vector_add PRIVATE webgpu_compute)

configure_file(vec-add.wglsl ${CMAKE_CURRENT_BINARY_DIR}/vec-add.wglsl COPYONLY)

