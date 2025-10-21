# GPU-Task

A toy cross-platform GPU computation engine that provides a simple abstraction layer for GPU computing using OpenCL.

## Features

- Cross-platform GPU computation support (Linux, macOS, Windows)
- OpenCL backend for broad hardware compatibility
- Simple C++ API for GPU buffer management and kernel execution
- Example application demonstrating vector addition on GPU

## Architecture

The library consists of three main components:

1. **GPUContext**: Manages GPU device initialization and context
2. **GPUBuffer**: Handles GPU memory allocation and data transfers
3. **GPUKernel**: Compiles and executes GPU kernels

## Building

### Prerequisites

- CMake 3.10 or higher
- C++17 compatible compiler
- OpenCL SDK (optional, but required for GPU functionality)

**Note:** The project will build without OpenCL, but will not be able to execute GPU kernels. Install OpenCL SDK for full functionality.

#### Installing OpenCL SDK

**Linux:**
```bash
# Ubuntu/Debian
sudo apt-get install opencl-headers ocl-icd-opencl-dev

# Fedora/RHEL
sudo dnf install opencl-headers ocl-icd-devel
```

**macOS:**
OpenCL is included by default in macOS frameworks.

**Windows:**
Install the OpenCL SDK from your GPU vendor (NVIDIA, AMD, or Intel).

### Build Instructions

```bash
mkdir build
cd build
cmake ..
make
```

## Usage

### Running the Example

After building, run the vector addition example:

```bash
./vector_add
```

This will perform element-wise addition of two 1024-element vectors on the GPU.

### API Example

```cpp
#include "gpu_context.h"
#include "gpu_buffer.h"
#include "gpu_kernel.h"

// Initialize GPU
gputask::GPUContext context;
context.initialize();

// Create buffers
gputask::GPUBuffer input(context, size * sizeof(float), true);
gputask::GPUBuffer output(context, size * sizeof(float), false);

// Write data
input.write(data.data(), size * sizeof(float));

// Create and compile kernel
gputask::GPUKernel kernel(context, kernel_source, "my_kernel");
kernel.compile();

// Set arguments and execute
kernel.setArg(0, input);
kernel.setArg(1, output);
kernel.execute(size);
kernel.finish();

// Read results
output.read(results.data(), size * sizeof(float));
```

## OpenCL Kernel Format

Kernels are written in OpenCL C:

```c
__kernel void my_kernel(__global const float* input,
                        __global float* output)
{
    int i = get_global_id(0);
    output[i] = input[i] * 2.0f;
}
```

## Limitations

This is a toy project for educational purposes. Production use would require:

- More robust error handling
- Support for multiple GPU backends (CUDA, Vulkan, Metal)
- Memory management optimizations
- Asynchronous operations
- Thread safety

## License

MIT License - See LICENSE file for details
