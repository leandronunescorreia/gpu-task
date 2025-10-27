#pragma once
#include <webgpu/webgpu.h>
#include <unordered_map>
#include <memory>
#include <string>
#include <vector>


struct ComputeEffect {
    std::string name;
    WGPUShaderModule shaderModule;
    WGPUBindGroupLayout bindGroupLayout;
    WGPUComputePipeline pipeline;
    std::vector<WGPUBuffer> buffers;
    WGPUBindGroup bindGroup;
};


class ComputeEffectStore {
public:
    ComputeEffectStore() = default;
    ~ComputeEffectStore() = default;


    void addEffect(const std::string& name, std::unique_ptr<ComputeEffect> effect) {
        effects[name] = std::move(effect);
    }

    ComputeEffect* get(const std::string& name) {
        auto it = effects.find(name);
        if (it != effects.end()) {
            return it->second.get();
        }
        return nullptr;
    }

private:
    std::unordered_map<std::string, std::unique_ptr<ComputeEffect>> effects;
};
