# Quick Start Guide

## Get GPU-Task Running in 5 Minutes

### Step 1: Build the Project

```bash
./build.sh
```

This will:
- Configure the project with CMake
- Build the GPU-Task library
- Compile all example applications

### Step 2: Check GPU Availability

```bash
cd build
./device_info
```

If you see "No GPU backend available", you need to install OpenCL:

**Linux (Ubuntu/Debian):**
```bash
sudo apt-get install opencl-headers ocl-icd-opencl-dev
```

**macOS:**
OpenCL is already included! Just rebuild.

**Windows:**
Install OpenCL from your GPU vendor (NVIDIA, AMD, or Intel).

Then rebuild:
```bash
cd ..
rm -rf build
./build.sh
```

### Step 3: Run Examples

```bash
cd build

# Check device info
./device_info

# Run vector addition
./vector_add

# Run scalar multiplication
./scalar_multiply
```

## What You'll See

With OpenCL installed, you'll see output like:

```
=== GPU Task: Vector Addition Example ===
GPU Device: Intel(R) HD Graphics (GPU)
Vector size: 1024 elements
Executing kernel on GPU...
Success! All results are correct.
Sample results:
  0 + 0 = 0
  1 + 2 = 3
  2 + 4 = 6
  ...
```

## Writing Your Own GPU Kernel

Create a new file `my_example.cpp`:

```cpp
#include "gputask.h"
#include <iostream>
#include <vector>

const char* my_kernel_source = R"(
__kernel void my_kernel(__global float* data) {
    int i = get_global_id(0);
    data[i] = data[i] * 2.0f;
}
)";

int main() {
    // Initialize
    gputask::GPUContext context;
    if (!context.initialize()) {
        std::cerr << "GPU not available!" << std::endl;
        return 1;
    }

    // Prepare data
    std::vector<float> data(1024, 1.0f);
    
    // Create buffer
    gputask::GPUBuffer buffer(context, data.size() * sizeof(float));
    buffer.write(data.data(), data.size() * sizeof(float));
    
    // Create and run kernel
    gputask::GPUKernel kernel(context, my_kernel_source, "my_kernel");
    kernel.compile();
    kernel.setArg(0, buffer);
    kernel.execute(data.size());
    kernel.finish();
    
    // Read results
    buffer.read(data.data(), data.size() * sizeof(float));
    
    std::cout << "First value: " << data[0] << std::endl;
    return 0;
}
```

Add to `CMakeLists.txt`:
```cmake
add_executable(my_example examples/my_example.cpp)
target_link_libraries(my_example gputask)
```

Rebuild and run!

## Need Help?

- Check [README.md](README.md) for detailed documentation
- See [IMPLEMENTATION.md](IMPLEMENTATION.md) for technical details
- Read [CONTRIBUTING.md](CONTRIBUTING.md) to contribute

## Common Issues

**"No GPU backend available"**
- Install OpenCL SDK for your platform
- Rebuild the project

**Compilation errors**
- Ensure you have C++17 compiler
- Check CMake version (need 3.10+)

**Runtime crashes**
- Verify your GPU drivers are up to date
- Check that your GPU supports OpenCL

## Next Steps

1. Read the full [README.md](README.md)
2. Explore the example source code in `examples/`
3. Try modifying the examples
4. Write your own GPU kernels!

Happy GPU computing! 🚀
