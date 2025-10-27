#pragma once
#include <webgpu/webgpu.h>
#include "webgpu_compute/compute_effects_store.h"
#include <vector>
#include <string>

class ComputeEngine
{
private:
    WGPUShaderModule createShaderModule(const std::string &wgslCode);

public:
    ComputeEngine(WGPUDevice device);
    ~ComputeEngine();

    WGPUComputePipeline initPipeline(const std::string &wgslPath,
                                     const std::vector<WGPUBuffer> &inputs,
                                     const std::vector<WGPUBuffer> &outputs,
                                     const uint64_t bufferSize);
    void run(const std::vector<WGPUBuffer> &inputs,
             const std::vector<WGPUBuffer> &outputs,
             const std::vector<ComputeEffect> &effects);

private:
    WGPUDevice m_device;
    WGPUBindGroupLayout m_bindGroupLayout;
    WGPUComputePipeline m_pipeline;
    WGPUBindGroup m_bindGroup;
};
