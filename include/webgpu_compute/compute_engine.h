#pragma once
#include <webgpu/webgpu.h>
#include <vector>
#include <string>

class ComputeEngine {
public:
    ComputeEngine(WGPUDevice device);
    ~ComputeEngine();

    void initPipeline(const std::string& wgslPath);
    void run(const std::vector<float>& input, std::vector<float>& output);

private:
    WGPUDevice m_device;
    WGPUBindGroupLayout m_bindGroupLayout;
    WGPUComputePipeline m_pipeline;
    WGPUBindGroup m_bindGroup;
};
