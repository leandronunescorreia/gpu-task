#include "gpu_kernel.h"
#include <iostream>
#include <cstring>

namespace gputask {

GPUKernel::GPUKernel(GPUContext& context, const std::string& source, const std::string& kernel_name)
    : context_(context), source_(source), kernel_name_(kernel_name), compiled_(false) {
#ifdef USE_OPENCL
    program_ = nullptr;
    kernel_ = nullptr;
#endif
}

GPUKernel::~GPUKernel() {
#ifdef USE_OPENCL
    if (kernel_) clReleaseKernel(kernel_);
    if (program_) clReleaseProgram(program_);
#endif
}

bool GPUKernel::compile() {
#ifdef USE_OPENCL
    if (!context_.isInitialized()) {
        std::cerr << "Error: GPUContext not initialized!" << std::endl;
        return false;
    }

    cl_int err;
    const char* source_str = source_.c_str();
    size_t source_size = source_.length();

    // Create program
    program_ = clCreateProgramWithSource(context_.getContext(), 1, &source_str, &source_size, &err);
    if (err != CL_SUCCESS) {
        std::cerr << "Error: Failed to create program!" << std::endl;
        return false;
    }

    // Build program
    err = clBuildProgram(program_, 1, &context_.getDevice(), nullptr, nullptr, nullptr);
    if (err != CL_SUCCESS) {
        char build_log[16384];
        clGetProgramBuildInfo(program_, context_.getDevice(), CL_PROGRAM_BUILD_LOG, 
                             sizeof(build_log), build_log, nullptr);
        std::cerr << "Error: Failed to build program!" << std::endl;
        std::cerr << "Build log: " << build_log << std::endl;
        clReleaseProgram(program_);
        program_ = nullptr;
        return false;
    }

    // Create kernel
    kernel_ = clCreateKernel(program_, kernel_name_.c_str(), &err);
    if (err != CL_SUCCESS) {
        std::cerr << "Error: Failed to create kernel!" << std::endl;
        clReleaseProgram(program_);
        program_ = nullptr;
        return false;
    }

    compiled_ = true;
    return true;
#else
    return false;
#endif
}

bool GPUKernel::setArg(unsigned int index, GPUBuffer& buffer) {
#ifdef USE_OPENCL
    if (!compiled_) {
        std::cerr << "Error: Kernel not compiled!" << std::endl;
        return false;
    }

    cl_mem buf = buffer.getBuffer();
    cl_int err = clSetKernelArg(kernel_, index, sizeof(cl_mem), &buf);
    
    if (err != CL_SUCCESS) {
        std::cerr << "Error: Failed to set kernel argument!" << std::endl;
        return false;
    }
    
    return true;
#else
    return false;
#endif
}

bool GPUKernel::setArg(unsigned int index, size_t size, const void* value) {
#ifdef USE_OPENCL
    if (!compiled_) {
        std::cerr << "Error: Kernel not compiled!" << std::endl;
        return false;
    }

    cl_int err = clSetKernelArg(kernel_, index, size, value);
    
    if (err != CL_SUCCESS) {
        std::cerr << "Error: Failed to set kernel argument!" << std::endl;
        return false;
    }
    
    return true;
#else
    return false;
#endif
}

bool GPUKernel::execute(size_t global_work_size, size_t local_work_size) {
#ifdef USE_OPENCL
    if (!compiled_) {
        std::cerr << "Error: Kernel not compiled!" << std::endl;
        return false;
    }

    size_t* local_size = (local_work_size > 0) ? &local_work_size : nullptr;
    
    cl_int err = clEnqueueNDRangeKernel(context_.getCommandQueue(), kernel_, 
                                        1, nullptr, &global_work_size, local_size, 
                                        0, nullptr, nullptr);
    
    if (err != CL_SUCCESS) {
        std::cerr << "Error: Failed to execute kernel!" << std::endl;
        return false;
    }
    
    return true;
#else
    return false;
#endif
}

bool GPUKernel::finish() {
#ifdef USE_OPENCL
    cl_int err = clFinish(context_.getCommandQueue());
    
    if (err != CL_SUCCESS) {
        std::cerr << "Error: Failed to finish kernel execution!" << std::endl;
        return false;
    }
    
    return true;
#else
    return false;
#endif
}

} // namespace gputask
