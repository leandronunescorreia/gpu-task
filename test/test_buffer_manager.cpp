#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "webgpu_compute/device_manager.h"
#include "webgpu_compute/buffer_manager.h"

using ::testing::NotNull;

TEST(BufferManagerTest, CREATE_WRITEONLY_BUFFER) {
    DeviceManager deviceManager = DeviceManager();
    // DeviceManager is a value type (not a pointer). We'll validate
    // functionality by checking the returned WebGPU handles below.

    WGPUInstance instance = deviceManager.getInstance();
    EXPECT_NE(instance, nullptr);

    WGPUAdapter adapter = deviceManager.createAdapter(instance);
    EXPECT_NE(adapter, nullptr);

    WGPUDevice device = deviceManager.createDevice(adapter);
    EXPECT_NE(device, nullptr);


    constexpr size_t bufferSize = 1024;
    WGPUBuffer storageBuffer = BufferManager::createStorageBuffer(device, bufferSize, false);
    EXPECT_NE(storageBuffer, nullptr);

    WGPUBufferUsage usage = wgpuBufferGetUsage(storageBuffer);

    WGPUBufferUsage expected = (WGPUBufferUsage)(WGPUBufferUsage_Storage | WGPUBufferUsage_CopyDst);
    EXPECT_EQ(usage & expected, expected);

    wgpuBufferDestroy(storageBuffer);
    wgpuAdapterRelease(adapter);
    wgpuInstanceRelease(instance);
    wgpuDeviceRelease(device);
}