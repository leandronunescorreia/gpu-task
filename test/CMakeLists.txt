include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()
include(GoogleTest)


add_executable(test_device_manager test_device_manager.cpp)
target_link_libraries(test_device_manager
  PRIVATE
    webgpu_compute
    GTest::gmock_main
)
gtest_discover_tests(test_device_manager)


add_executable(test_buffer_manager test_buffer_manager.cpp)
target_link_libraries(test_buffer_manager
  PRIVATE
    webgpu_compute
    GTest::gmock_main
)
gtest_discover_tests(test_buffer_manager)

# Convenience target to build all test executables in one command.
# Usage: cmake --build build --target build_tests -- -j$(nproc)
add_custom_target(build_tests
  DEPENDS
    test_device_manager
    test_buffer_manager
)
