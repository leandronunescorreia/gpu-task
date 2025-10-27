#include "webgpu_compute/buffer_manager.h"
#include <vector>


WGPUBuffer BufferManager::createStorageBuffer(WGPUDevice device, uint64_t size, bool readable) {
    WGPUBufferDescriptor desc = {};
    desc.size = size;
    desc.usage = WGPUBufferUsage_Storage | WGPUBufferUsage_CopyDst | WGPUBufferUsage_CopySrc;

    if (readable)
        desc.usage |= WGPUBufferUsage_MapRead;

    return wgpuDeviceCreateBuffer(device, &desc);
}

void BufferManager::writeBuffer(WGPUDevice device, WGPUBuffer buffer, const void* data, size_t size) {
    WGPUQueue queue = wgpuDeviceGetQueue(device);
    wgpuQueueWriteBuffer(queue, buffer, 0, data, size);
    wgpuQueueRelease(queue);
}


WGPUBindGroupLayout BufferManager::createBindGroupLayout(
    WGPUDevice device,
    size_t numReadOnlyBuffers,
    size_t numWritableBuffers)
{
    std::vector<WGPUBindGroupLayoutEntry> entries;
    entries.reserve(numReadOnlyBuffers + numWritableBuffers);

    uint32_t binding = 0;
    for (size_t i = 0; i < numReadOnlyBuffers; ++i, ++binding) {
        WGPUBindGroupLayoutEntry entry = {};
        entry.binding = binding;
        entry.visibility = WGPUShaderStage_Compute;
        entry.buffer.type = WGPUBufferBindingType_ReadOnlyStorage;
        entries.push_back(entry);
    }

    for (size_t i = 0; i < numWritableBuffers; ++i, ++binding) {
        WGPUBindGroupLayoutEntry entry = {};
        entry.binding = binding;
        entry.visibility = WGPUShaderStage_Compute;
        entry.buffer.type = WGPUBufferBindingType_Storage;
        entries.push_back(entry);
    }

    WGPUBindGroupLayoutDescriptor desc = {};
    desc.entryCount = static_cast<uint32_t>(entries.size());
    desc.entries = entries.data();

    return wgpuDeviceCreateBindGroupLayout(device, &desc);
}

WGPUBindGroup BufferManager::createBindGroup(
    WGPUDevice device,
    WGPUBindGroupLayout layout,
    const std::vector<WGPUBuffer>& readOnlyBuffers,
    const std::vector<WGPUBuffer>& writableBuffers,
    uint64_t bufferSize)
{
    std::vector<WGPUBindGroupEntry> entries;
    entries.reserve(readOnlyBuffers.size() + writableBuffers.size());

    uint32_t binding = 0;
    for (auto buf : readOnlyBuffers) {
        WGPUBindGroupEntry entry = {};
        entry.binding = binding++;
        entry.buffer = buf;
        entry.offset = 0;
        entry.size = bufferSize;
        entries.push_back(entry);
    }

    for (auto buf : writableBuffers) {
        WGPUBindGroupEntry entry = {};
        entry.binding = binding++;
        entry.buffer = buf;
        entry.offset = 0;
        entry.size = bufferSize;
        entries.push_back(entry);
    }

    WGPUBindGroupDescriptor groupDesc = {};
    groupDesc.layout = layout;
    groupDesc.entryCount = static_cast<uint32_t>(entries.size());
    groupDesc.entries = entries.data();

    return wgpuDeviceCreateBindGroup(device, &groupDesc);
}