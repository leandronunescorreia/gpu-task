add_library(webgpu_compute STATIC
    device_manager.cpp    
    buffer_manager.cpp
    compute_engine.cpp
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(webgpu_compute PRIVATE DEBUG_BUILD _DEBUG)
endif()

target_include_directories(webgpu_compute
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>   # during build
        $<INSTALL_INTERFACE:include>                     # after install
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}                      # for internal includes
)

# === Link with Dawn / WebGPU ===
target_link_libraries(webgpu_compute
    PUBLIC
        webgpu
)

## === Installation rules ===
# Skip installation/export when building tests in-source to avoid export
# dependency checks against external/prebuilt WebGPU targets which may
# not be part of an export set. Installing is primarily needed for
# packaging/use-after-install scenarios, not for running tests.
if (NOT BUILD_TESTS)
    install(TARGETS webgpu_compute
        EXPORT webgpu_computeTargets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
    )

    # === Install public headers ===
    # This keeps your folder structure include/webgpu_compute/*.h intact
    install(
        DIRECTORY ${CMAKE_SOURCE_DIR}/include/webgpu_compute/
        DESTINATION include/webgpu_compute
        FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
    )

    # === Export CMake package config ===
    install(
        EXPORT webgpu_computeTargets
        FILE webgpu_computeTargets.cmake
        NAMESPACE webgpu_compute::
        DESTINATION lib/cmake/webgpu_compute
    )
endif()
