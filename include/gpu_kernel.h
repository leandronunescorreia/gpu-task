#pragma once

#include "gpu_context.h"
#include "gpu_buffer.h"
#include <string>
#include <vector>

namespace gputask {

class GPUKernel {
public:
    GPUKernel(GPUContext& context, const std::string& source, const std::string& kernel_name);
    ~GPUKernel();

    bool compile();
    bool setArg(unsigned int index, GPUBuffer& buffer);
    bool setArg(unsigned int index, size_t size, const void* value);
    bool execute(size_t global_work_size, size_t local_work_size = 0);
    bool finish();

private:
    GPUContext& context_;
    std::string source_;
    std::string kernel_name_;
    bool compiled_;

#ifdef USE_OPENCL
    cl_program program_;
    cl_kernel kernel_;
#endif
};

} // namespace gputask
