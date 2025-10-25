#include "webgpu_compute/buffer_manager.h"

WGPUBuffer BufferManager::createStorageBuffer(WGPUDevice device, uint64_t size, bool readable) {
    WGPUBufferDescriptor desc = {};
    desc.size = size;
    desc.usage = readable
        ? (WGPUBufferUsage_Storage | WGPUBufferUsage_CopySrc | WGPUBufferUsage_MapRead)
        : (WGPUBufferUsage_Storage | WGPUBufferUsage_CopyDst);
    return wgpuDeviceCreateBuffer(device, &desc);
}

void BufferManager::writeBuffer(WGPUDevice device, WGPUBuffer buffer, const void* data, size_t size) {
    WGPUQueue queue = wgpuDeviceGetQueue(device);
    wgpuQueueWriteBuffer(queue, buffer, 0, data, size);
    wgpuQueueRelease(queue);
}
