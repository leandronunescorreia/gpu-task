#include <gtest/gtest.h>
#include "webgpu_compute/device_manager.h"


TEST(DeviceManagerTest, InitializeAndRun) {
    DeviceManager deviceManager;
    WGPUInstance instance = deviceManager.getInstance();

    EXPECT_NE(instance, nullptr);
}
