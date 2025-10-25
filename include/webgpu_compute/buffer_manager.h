#pragma once
#include <webgpu/webgpu.h>

class BufferManager {
public:
    static WGPUBuffer createStorageBuffer(WGPUDevice device, uint64_t size, bool readable);
    static void writeBuffer(WGPUDevice device, WGPUBuffer buffer, const void* data, size_t size);
};
