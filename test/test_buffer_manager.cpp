#include <gtest/gtest.h>
#include "webgpu_compute/device_manager.h"
#include "webgpu_compute/buffer_manager.h"


TEST(BufferManagerTest, CREATE_WRITEONLY_BUFFER) {
    DeviceManager deviceManager = DeviceManager();
    EXPECT_NE(deviceManager, nullptr);

    auto instance = deviceManager.getInstance();
    EXPECT_NE(instance, nullptr);

    auto adapter = deviceManager.createAdapter(instance);
    EXPECT_NE(adapter, nullptr);

    auto device = deviceManager.createDevice(adapter);
    EXPECT_NE(device, nullptr);


    constexpr size_t bufferSize = 1024;
    WGPUBuffer storageBuffer = BufferManager::createStorageBuffer(device, bufferSize, false);
    EXPECT_NE(storageBuffer, nullptr);

    auto usage = wgpuBufferGetUsage(storageBuffer);

    auto expected = (WGPUBufferUsage)(WGPUBufferUsage_Storage | WGPUBufferUsage_CopyDst);
    EXPECT_EQ(usage & expected, expected);

    wgpuBufferDestroy(storageBuffer);
    wgpuAdapterRelease(adapter);
    wgpuInstanceRelease(instance);
    wgpuDeviceRelease(device);
}