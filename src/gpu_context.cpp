#include "gpu_context.h"
#include <iostream>
#include <cstring>

namespace gputask {

GPUContext::GPUContext() : initialized_(false) {
#ifdef USE_OPENCL
    platform_id_ = nullptr;
    device_id_ = nullptr;
    context_ = nullptr;
    command_queue_ = nullptr;
#endif
}

GPUContext::~GPUContext() {
#ifdef USE_OPENCL
    if (command_queue_) clReleaseCommandQueue(command_queue_);
    if (context_) clReleaseContext(context_);
#endif
}

bool GPUContext::initialize() {
#ifdef USE_OPENCL
    cl_int err;
    cl_uint num_platforms;
    
    // Get platform
    err = clGetPlatformIDs(1, &platform_id_, &num_platforms);
    if (err != CL_SUCCESS || num_platforms == 0) {
        std::cerr << "Error: Failed to find OpenCL platform!" << std::endl;
        return false;
    }

    // Get device
    cl_uint num_devices;
    err = clGetDeviceIDs(platform_id_, CL_DEVICE_TYPE_GPU, 1, &device_id_, &num_devices);
    if (err != CL_SUCCESS) {
        // Try CPU if GPU is not available
        err = clGetDeviceIDs(platform_id_, CL_DEVICE_TYPE_CPU, 1, &device_id_, &num_devices);
        if (err != CL_SUCCESS) {
            std::cerr << "Error: Failed to find OpenCL device!" << std::endl;
            return false;
        }
    }

    // Create context
    context_ = clCreateContext(nullptr, 1, &device_id_, nullptr, nullptr, &err);
    if (err != CL_SUCCESS) {
        std::cerr << "Error: Failed to create OpenCL context!" << std::endl;
        return false;
    }

    // Create command queue
    command_queue_ = clCreateCommandQueue(context_, device_id_, 0, &err);
    if (err != CL_SUCCESS) {
        std::cerr << "Error: Failed to create command queue!" << std::endl;
        clReleaseContext(context_);
        context_ = nullptr;
        return false;
    }

    initialized_ = true;
    return true;
#else
    std::cerr << "Error: No GPU backend available!" << std::endl;
    return false;
#endif
}

std::string GPUContext::getDeviceInfo() const {
#ifdef USE_OPENCL
    if (!initialized_) {
        return "Context not initialized";
    }

    char device_name[1024];
    clGetDeviceInfo(device_id_, CL_DEVICE_NAME, sizeof(device_name), device_name, nullptr);
    
    char device_vendor[1024];
    clGetDeviceInfo(device_id_, CL_DEVICE_VENDOR, sizeof(device_vendor), device_vendor, nullptr);
    
    cl_device_type device_type;
    clGetDeviceInfo(device_id_, CL_DEVICE_TYPE, sizeof(device_type), &device_type, nullptr);
    
    std::string type_str = (device_type == CL_DEVICE_TYPE_GPU) ? "GPU" : "CPU";
    
    return std::string(device_vendor) + " " + std::string(device_name) + " (" + type_str + ")";
#else
    return "No GPU backend available";
#endif
}

} // namespace gputask
