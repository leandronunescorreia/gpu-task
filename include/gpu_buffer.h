#pragma once

#include "gpu_context.h"
#include <cstddef>
#include <memory>

namespace gputask {

class GPUBuffer {
public:
    GPUBuffer(GPUContext& context, size_t size, bool read_only = false);
    ~GPUBuffer();

    bool write(const void* data, size_t size);
    bool read(void* data, size_t size);
    
    size_t size() const { return size_; }

#ifdef USE_OPENCL
    cl_mem getBuffer() const { return buffer_; }
#endif

private:
    GPUContext& context_;
    size_t size_;
    bool read_only_;

#ifdef USE_OPENCL
    cl_mem buffer_;
#endif
};

} // namespace gputask
