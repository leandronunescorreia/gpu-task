#pragma once

#ifdef USE_OPENCL
#define CL_TARGET_OPENCL_VERSION 120
#include <CL/cl.h>
#endif

#include <memory>
#include <string>
#include <vector>

namespace gputask {

class GPUContext {
public:
    GPUContext();
    ~GPUContext();

    bool initialize();
    bool isInitialized() const { return initialized_; }
    
    std::string getDeviceInfo() const;

#ifdef USE_OPENCL
    cl_context getContext() const { return context_; }
    cl_command_queue getCommandQueue() const { return command_queue_; }
    cl_device_id getDevice() const { return device_id_; }
#endif

private:
    bool initialized_;

#ifdef USE_OPENCL
    cl_platform_id platform_id_;
    cl_device_id device_id_;
    cl_context context_;
    cl_command_queue command_queue_;
#endif
};

} // namespace gputask
