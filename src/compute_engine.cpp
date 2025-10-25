#include "webgpu_compute/compute_engine.h"
#include "webgpu_compute/buffer_manager.h"
#include <fstream>
#include <iostream>
#include <cstring>

static std::string readFile(const std::string& path) {
    std::ifstream file(path);
    return std::string((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
}

ComputeEngine::ComputeEngine(WGPUDevice device)
    : m_device(device) {}

ComputeEngine::~ComputeEngine() {
    wgpuBindGroupLayoutRelease(m_bindGroupLayout);
    wgpuComputePipelineRelease(m_pipeline);
    wgpuBindGroupRelease(m_bindGroup);
}

void ComputeEngine::initPipeline(const std::string& wgslPath) {
    (void)wgslPath;
    // ...  
}

void ComputeEngine::run(const std::vector<float>& input, std::vector<float>& output) {
    (void)input;
    (void)output;
}
