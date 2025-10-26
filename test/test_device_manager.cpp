#include <gtest/gtest.h>
#include "webgpu_compute/device_manager.h"


TEST(DeviceManagerTest, InitializeAndRun) {
    DeviceManager deviceManager;
    WGPUInstance instance = deviceManager.getInstance();

    EXPECT_NE(instance, nullptr);
}


TEST(DeviceManagerTest, CreateAdapterAndDevice) {
    DeviceManager deviceManager;
    WGPUInstance instance = deviceManager.getInstance();

    EXPECT_NE(instance, nullptr);

    WGPUAdapter adapter = deviceManager.createAdapter(instance);
    EXPECT_NE(adapter, nullptr);

    WGPUDevice device = deviceManager.createDevice(adapter);
    EXPECT_NE(device, nullptr);
}


TEST(DeviceManagerTest, QueryDeviceLimits) {
    DeviceManager deviceManager;
    WGPUInstance instance = deviceManager.getInstance();

    EXPECT_NE(instance, nullptr);

    WGPUAdapter adapter = deviceManager.createAdapter(instance);
    EXPECT_NE(adapter, nullptr);

    WGPUDevice device = deviceManager.createDevice(adapter);
    EXPECT_NE(device, nullptr);

    WGPULimits limits = deviceManager.queryDeviceLimits(device);
    // Basic sanity checks: many limits should be positive/non-zero
    EXPECT_GT(limits.maxTextureDimension2D, 0u);
    EXPECT_GT(limits.maxBindGroups, 0u);
}
