#pragma once
#include <webgpu/webgpu.h>
#include <vector>

class BufferManager {
public:
    static WGPUBuffer createStorageBuffer(WGPUDevice device, uint64_t size, bool readable);
    static void writeBuffer(WGPUDevice device, WGPUBuffer buffer, const void* data, size_t size);

    static WGPUBindGroupLayout createBindGroupLayout(
        WGPUDevice device,
        size_t numReadOnlyBuffers,
        size_t numWritableBuffers);

    static WGPUBindGroup createBindGroup(
        WGPUDevice device,
        WGPUBindGroupLayout layout,
        const std::vector<WGPUBuffer>& readOnlyBuffers,
        const std::vector<WGPUBuffer>& writableBuffers,
        uint64_t bufferSize);    
};
