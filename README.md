# gpu-task
cross-platform gpu computation engine


# initial build commands

cmake -S . -B build -DBUILD_SAMPLES=OFF
cmake --build build --target webgpu_compute -- -j$(nproc)