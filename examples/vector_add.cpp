#include "gpu_context.h"
#include "gpu_buffer.h"
#include "gpu_kernel.h"
#include <iostream>
#include <vector>
#include <cmath>

// OpenCL kernel for vector addition
const char* vector_add_kernel = R"(
__kernel void vector_add(__global const float* a,
                         __global const float* b,
                         __global float* c,
                         const unsigned int n)
{
    int i = get_global_id(0);
    if (i < n) {
        c[i] = a[i] + b[i];
    }
}
)";

int main() {
    std::cout << "=== GPU Task: Vector Addition Example ===" << std::endl;
    
    // Initialize GPU context
    gputask::GPUContext context;
    if (!context.initialize()) {
        std::cerr << "Failed to initialize GPU context!" << std::endl;
        return 1;
    }
    
    std::cout << "GPU Device: " << context.getDeviceInfo() << std::endl;
    
    // Create test vectors
    const size_t vector_size = 1024;
    std::vector<float> a(vector_size);
    std::vector<float> b(vector_size);
    std::vector<float> c(vector_size);
    
    // Initialize input vectors
    for (size_t i = 0; i < vector_size; ++i) {
        a[i] = static_cast<float>(i);
        b[i] = static_cast<float>(i * 2);
    }
    
    std::cout << "Vector size: " << vector_size << " elements" << std::endl;
    
    // Create GPU buffers
    gputask::GPUBuffer buffer_a(context, vector_size * sizeof(float), true);
    gputask::GPUBuffer buffer_b(context, vector_size * sizeof(float), true);
    gputask::GPUBuffer buffer_c(context, vector_size * sizeof(float), false);
    
    // Write input data to GPU
    if (!buffer_a.write(a.data(), a.size() * sizeof(float))) {
        std::cerr << "Failed to write buffer A!" << std::endl;
        return 1;
    }
    
    if (!buffer_b.write(b.data(), b.size() * sizeof(float))) {
        std::cerr << "Failed to write buffer B!" << std::endl;
        return 1;
    }
    
    // Create and compile kernel
    gputask::GPUKernel kernel(context, vector_add_kernel, "vector_add");
    if (!kernel.compile()) {
        std::cerr << "Failed to compile kernel!" << std::endl;
        return 1;
    }
    
    // Set kernel arguments
    unsigned int n = vector_size;
    kernel.setArg(0, buffer_a);
    kernel.setArg(1, buffer_b);
    kernel.setArg(2, buffer_c);
    kernel.setArg(3, sizeof(unsigned int), &n);
    
    std::cout << "Executing kernel on GPU..." << std::endl;
    
    // Execute kernel
    if (!kernel.execute(vector_size)) {
        std::cerr << "Failed to execute kernel!" << std::endl;
        return 1;
    }
    
    // Wait for completion
    kernel.finish();
    
    // Read results from GPU
    if (!buffer_c.read(c.data(), c.size() * sizeof(float))) {
        std::cerr << "Failed to read buffer C!" << std::endl;
        return 1;
    }
    
    // Verify results
    bool success = true;
    int errors = 0;
    for (size_t i = 0; i < vector_size && errors < 10; ++i) {
        float expected = a[i] + b[i];
        if (std::fabs(c[i] - expected) > 0.001f) {
            std::cerr << "Error at index " << i << ": expected " << expected 
                      << ", got " << c[i] << std::endl;
            success = false;
            errors++;
        }
    }
    
    if (success) {
        std::cout << "Success! All results are correct." << std::endl;
        std::cout << "Sample results:" << std::endl;
        for (size_t i = 0; i < 10; ++i) {
            std::cout << "  " << a[i] << " + " << b[i] << " = " << c[i] << std::endl;
        }
    } else {
        std::cerr << "Verification failed!" << std::endl;
        return 1;
    }
    
    return 0;
}
