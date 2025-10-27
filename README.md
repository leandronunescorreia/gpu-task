# gpu-task --- WIP ---
cross-platform gpu computation engine

A small, cross-platform GPU compute engine and sample collection for prototyping and
benchmarking compute kernels using WebGPU-compatible backends. The project provides
clean C++ wrappers and buildable samples so you can quickly test GPU compute
workloads on Windows, Linux and macOS.

Key features:

- Lightweight, header/source C++ implementation with a focus on compute workflows.
- Sample programs (e.g., vector addition) and simple build configuration via CMake.
- Supports WebGPU backends (Dawn / wgpu-native) to keep parity across platforms.
- Test targets included to help validate device and buffer management.

See the sections below for build, install and test commands.


# initial cmake config command

```bash
cmake -S . -B build -DBUILD_SAMPLES=OFF
```

# initial build command
```bash
cmake --build build --target webgpu_compute -- -j$(nproc)
```


# for install build

> configure
```bash
cmake -S . -B build -DBUILD_SAMPLES=OFF -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=install
```
> build
# initial build command
```bash
cmake --build build --target webgpu_compute -- -j$(nproc)
```

> install
```bash
cmake --install build
```




# to run tests

### config build for tests
```bash
cmake -S . -B build -DBUILD_TESTS=ON -DWEBGPU_BACKEND=DAWN -DWEBGPU_BUILD_FROM_SOURCE=OFF -DBUILD_SAMPLES=OFF
```

### build test
```bash
cmake --build build --target test_device_manager
```

### build all tests
```bash
cmake --build build --target test_device_manager test_buffer_manager -- -j$(nproc)
```

### run the test agregator
```bash
cmake --build build --target build_tests -- -j$(nproc)
```

### run tests
```bash
./build/test/test_device_manager && ./build/test/test_buffer_manager
```


### build all
```bash
cmake --build build -- -j$(nproc)
```