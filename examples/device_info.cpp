#include "gpu_context.h"
#include <iostream>

int main() {
    std::cout << "=== GPU Task: Device Information ===" << std::endl;
    std::cout << std::endl;
    
    // Try to initialize GPU context
    gputask::GPUContext context;
    
    if (!context.initialize()) {
        std::cout << "Status: No GPU backend available" << std::endl;
        std::cout << std::endl;
        std::cout << "To enable GPU functionality:" << std::endl;
        std::cout << "  1. Install OpenCL SDK for your platform" << std::endl;
        std::cout << "  2. Rebuild the project with: ./build.sh" << std::endl;
        std::cout << std::endl;
        std::cout << "Installation guides:" << std::endl;
        std::cout << "  Linux:   sudo apt-get install opencl-headers ocl-icd-opencl-dev" << std::endl;
        std::cout << "  macOS:   OpenCL is included by default" << std::endl;
        std::cout << "  Windows: Install GPU vendor SDK (NVIDIA/AMD/Intel)" << std::endl;
        return 1;
    }
    
    std::cout << "Status: GPU backend initialized successfully" << std::endl;
    std::cout << "Device: " << context.getDeviceInfo() << std::endl;
    std::cout << std::endl;
    std::cout << "You can now run GPU computation examples:" << std::endl;
    std::cout << "  ./vector_add" << std::endl;
    std::cout << "  ./scalar_multiply" << std::endl;
    
    return 0;
}
