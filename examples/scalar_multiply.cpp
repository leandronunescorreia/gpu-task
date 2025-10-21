#include "gpu_context.h"
#include "gpu_buffer.h"
#include "gpu_kernel.h"
#include <iostream>
#include <vector>

// OpenCL kernel for scalar multiplication
const char* scalar_multiply_kernel = R"(
__kernel void scalar_multiply(__global const float* input,
                               __global float* output,
                               const float scalar,
                               const unsigned int n)
{
    int i = get_global_id(0);
    if (i < n) {
        output[i] = input[i] * scalar;
    }
}
)";

int main() {
    std::cout << "=== GPU Task: Scalar Multiplication Example ===" << std::endl;
    
    // Initialize GPU context
    gputask::GPUContext context;
    if (!context.initialize()) {
        std::cerr << "Failed to initialize GPU context!" << std::endl;
        std::cerr << "Note: This example requires OpenCL to be installed." << std::endl;
        return 1;
    }
    
    std::cout << "GPU Device: " << context.getDeviceInfo() << std::endl;
    
    // Create test vector
    const size_t vector_size = 512;
    const float scalar = 3.14159f;
    std::vector<float> input(vector_size);
    std::vector<float> output(vector_size);
    
    // Initialize input vector
    for (size_t i = 0; i < vector_size; ++i) {
        input[i] = static_cast<float>(i + 1);
    }
    
    std::cout << "Vector size: " << vector_size << " elements" << std::endl;
    std::cout << "Scalar: " << scalar << std::endl;
    
    // Create GPU buffers
    gputask::GPUBuffer buffer_in(context, vector_size * sizeof(float), true);
    gputask::GPUBuffer buffer_out(context, vector_size * sizeof(float), false);
    
    // Write input data to GPU
    buffer_in.write(input.data(), input.size() * sizeof(float));
    
    // Create and compile kernel
    gputask::GPUKernel kernel(context, scalar_multiply_kernel, "scalar_multiply");
    if (!kernel.compile()) {
        std::cerr << "Failed to compile kernel!" << std::endl;
        return 1;
    }
    
    // Set kernel arguments
    unsigned int n = vector_size;
    kernel.setArg(0, buffer_in);
    kernel.setArg(1, buffer_out);
    kernel.setArg(2, sizeof(float), &scalar);
    kernel.setArg(3, sizeof(unsigned int), &n);
    
    std::cout << "Executing kernel on GPU..." << std::endl;
    
    // Execute kernel
    kernel.execute(vector_size);
    kernel.finish();
    
    // Read results from GPU
    buffer_out.read(output.data(), output.size() * sizeof(float));
    
    // Verify and display results
    std::cout << "Success! Sample results:" << std::endl;
    for (size_t i = 0; i < 5; ++i) {
        std::cout << "  " << input[i] << " * " << scalar << " = " << output[i] << std::endl;
    }
    
    return 0;
}
