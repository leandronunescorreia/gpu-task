cmake_minimum_required(VERSION 3.10)
project(gpu-task VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenCL
find_package(OpenCL)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

if(OpenCL_FOUND)
    include_directories(${OpenCL_INCLUDE_DIRS})
    add_definitions(-DUSE_OPENCL)
endif()

# GPU Task library
add_library(gputask STATIC
    src/gpu_context.cpp
    src/gpu_buffer.cpp
    src/gpu_kernel.cpp
)

if(OpenCL_FOUND)
    target_link_libraries(gputask ${OpenCL_LIBRARIES})
endif()

# Example application
add_executable(vector_add examples/vector_add.cpp)
target_link_libraries(vector_add gputask)

if(OpenCL_FOUND)
    target_link_libraries(vector_add ${OpenCL_LIBRARIES})
endif()

# Installation rules
install(TARGETS gputask vector_add
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/ DESTINATION include)
