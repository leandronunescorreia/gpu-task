#pragma once
#include <webgpu/webgpu.h>

class DeviceManager {
public:
    DeviceManager();
    ~DeviceManager();

    WGPUAdapter createAdapter(WGPUInstance instance);
    WGPUDevice createDevice(WGPUAdapter adapter);
    WGPULimits queryDeviceLimits(WGPUDevice device);

    WGPUInstance getInstance() const { return m_instance; }
    WGPUDevice getDevice() const { return m_device; }

private:
    WGPUInstance m_instance = nullptr;
    WGPUDevice m_device = nullptr;
    WGPULimits m_limits = {};
};
