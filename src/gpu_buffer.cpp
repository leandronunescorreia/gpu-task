#include "gpu_buffer.h"
#include <iostream>
#include <cstring>

namespace gputask {

GPUBuffer::GPUBuffer(GPUContext& context, size_t size, bool read_only)
    : context_(context), size_(size), read_only_(read_only) {
#ifdef USE_OPENCL
    buffer_ = nullptr;
    
    if (!context_.isInitialized()) {
        std::cerr << "Error: GPUContext not initialized!" << std::endl;
        return;
    }

    cl_int err;
    cl_mem_flags flags = read_only_ ? CL_MEM_READ_ONLY : CL_MEM_READ_WRITE;
    buffer_ = clCreateBuffer(context_.getContext(), flags, size_, nullptr, &err);
    
    if (err != CL_SUCCESS) {
        std::cerr << "Error: Failed to create buffer!" << std::endl;
        buffer_ = nullptr;
    }
#endif
}

GPUBuffer::~GPUBuffer() {
#ifdef USE_OPENCL
    if (buffer_) {
        clReleaseMemObject(buffer_);
    }
#endif
}

bool GPUBuffer::write(const void* data, size_t size) {
#ifdef USE_OPENCL
    if (!buffer_) {
        std::cerr << "Error: Buffer not created!" << std::endl;
        return false;
    }

    if (size > size_) {
        std::cerr << "Error: Write size exceeds buffer size!" << std::endl;
        return false;
    }

    cl_int err = clEnqueueWriteBuffer(context_.getCommandQueue(), buffer_, 
                                      CL_TRUE, 0, size, data, 0, nullptr, nullptr);
    
    if (err != CL_SUCCESS) {
        std::cerr << "Error: Failed to write to buffer!" << std::endl;
        return false;
    }
    
    return true;
#else
    return false;
#endif
}

bool GPUBuffer::read(void* data, size_t size) {
#ifdef USE_OPENCL
    if (!buffer_) {
        std::cerr << "Error: Buffer not created!" << std::endl;
        return false;
    }

    if (size > size_) {
        std::cerr << "Error: Read size exceeds buffer size!" << std::endl;
        return false;
    }

    cl_int err = clEnqueueReadBuffer(context_.getCommandQueue(), buffer_, 
                                     CL_TRUE, 0, size, data, 0, nullptr, nullptr);
    
    if (err != CL_SUCCESS) {
        std::cerr << "Error: Failed to read from buffer!" << std::endl;
        return false;
    }
    
    return true;
#else
    return false;
#endif
}

} // namespace gputask
