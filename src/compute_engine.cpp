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

WGPUShaderModule ComputeEngine::createShaderModule(const std::string &wgslPath) {
    const std::string shaderCode = readFile(wgslPath);

    WGPUShaderModuleWGSLDescriptor wgslDesc = {};
    wgslDesc.chain = {};
    wgslDesc.chain.sType = WGPUSType_ShaderSourceWGSL;
    wgslDesc.chain.next = nullptr;
    wgslDesc.code = WGPUStringView{shaderCode.c_str(), static_cast<uint32_t>(shaderCode.length())};

    WGPUShaderModuleDescriptor smDesc = {};
    smDesc.label = WGPUStringView{"Compute Shader Module", static_cast<uint32_t>(std::strlen("Compute Shader Module"))};
    smDesc.nextInChain = reinterpret_cast<WGPUChainedStruct*>(&wgslDesc);
    WGPUShaderModule shaderModule = wgpuDeviceCreateShaderModule(this->m_device, &smDesc);
    return shaderModule;
}

WGPUComputePipeline ComputeEngine::initPipeline(const std::string &wgslPath,
                      const std::vector<WGPUBuffer> &inputs,
                      const std::vector<WGPUBuffer> &outputs,
                      const uint64_t bufferSize) {
    
    WGPUShaderModule shaderModule = createShaderModule(wgslPath);

    m_bindGroupLayout = BufferManager::createBindGroupLayout(
        m_device, inputs.size(), outputs.size());

    m_bindGroup = BufferManager::createBindGroup(
        m_device, m_bindGroupLayout, inputs, outputs, bufferSize);

    WGPUPipelineLayoutDescriptor pipelineLayoutDesc = {};
    pipelineLayoutDesc.nextInChain = nullptr;
    pipelineLayoutDesc.label = WGPUStringView{"Compute Pipeline Layout", static_cast<uint32_t>(std::strlen("Compute Pipeline Layout"))};
    pipelineLayoutDesc.bindGroupLayoutCount = 1;
    pipelineLayoutDesc.bindGroupLayouts = &this->m_bindGroupLayout;
    WGPUPipelineLayout pipelineLayout = wgpuDeviceCreatePipelineLayout(this->m_device, &pipelineLayoutDesc);


    WGPUConstantEntry constantEntry = {};
    constantEntry.nextInChain = nullptr;
    constantEntry.key = WGPUStringView{"group_size", static_cast<uint32_t>(std::strlen("group_size"))};
    constantEntry.value = static_cast<double>(32.0);

    WGPUComputeState computeState = {};
    computeState.nextInChain = nullptr;
    computeState.module = shaderModule;
    computeState.entryPoint = WGPUStringView{"computeMain", static_cast<uint32_t>(std::strlen("computeMain"))};
    computeState.constants = &constantEntry;
    computeState.constantCount = 1;

    WGPUComputePipelineDescriptor pipelineDesc = {};
    pipelineDesc.nextInChain = nullptr;
    pipelineDesc.label = WGPUStringView{"Compute Pipeline", static_cast<uint32_t>(std::strlen("Compute Pipeline"))};
    pipelineDesc.layout = pipelineLayout;
    pipelineDesc.compute = computeState;

    WGPUComputePipeline pipeline = wgpuDeviceCreateComputePipeline(this->m_device, &pipelineDesc);
    return pipeline;
}

void ComputeEngine::run(const std::vector<WGPUBuffer>& inputs,
                        const std::vector<WGPUBuffer>& outputs,
                        const std::vector<ComputeEffect> &effects) {
    (void)inputs;
    (void)outputs;
    (void)effects;
}
