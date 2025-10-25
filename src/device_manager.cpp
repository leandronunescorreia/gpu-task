#include "webgpu_compute/device_manager.h"
#include <iostream>
#include <stdexcept>

DeviceManager::DeviceManager() {
    WGPUInstanceDescriptor desc = {};
    m_instance = wgpuCreateInstance(&desc);
    if (!m_instance)
        throw std::runtime_error("Failed to create WebGPU instance");
}

DeviceManager::~DeviceManager() {
    if (m_device) wgpuDeviceRelease(m_device);
    if (m_instance) wgpuInstanceRelease(m_instance);
}

WGPUAdapter DeviceManager::createAdapter(WGPUInstance instance) {
    WGPURequestAdapterOptions adapterOpts = {};
    adapterOpts.backendType = WGPUBackendType_Vulkan;
    WGPUAdapter adapter = {};

    auto callback = [](WGPURequestAdapterStatus status, WGPUAdapter a, WGPUStringView, void* userdata1, void*) {
        if (status == WGPURequestAdapterStatus_Success)
            *reinterpret_cast<WGPUAdapter*>(userdata1) = a;
    };

    WGPURequestAdapterCallbackInfo cb = {};
    cb.callback = callback;
    cb.userdata1 = &adapter;
    wgpuInstanceRequestAdapter(instance, &adapterOpts, cb);

    if (!adapter)
        throw std::runtime_error("Failed to get WebGPU adapter");
    return adapter;
}

WGPUDevice DeviceManager::createDevice(WGPUAdapter adapter) {
    WGPUDeviceDescriptor desc = {};
    desc.defaultQueue.label = WGPUStringView{"Default Queue", 13};
    WGPUDevice device = {};

    auto cb = [](WGPURequestDeviceStatus status, WGPUDevice d, WGPUStringView, void* userdata1, void*) {
        if (status == WGPURequestDeviceStatus_Success)
            *reinterpret_cast<WGPUDevice*>(userdata1) = d;
    };

    WGPURequestDeviceCallbackInfo info = {};
    info.callback = cb;
    info.userdata1 = &device;
    wgpuAdapterRequestDevice(adapter, &desc, info);

    if (!device)
        throw std::runtime_error("Failed to create WebGPU device");

    m_device = device;
    wgpuAdapterRelease(adapter);
    return device;
}

WGPULimits DeviceManager::queryDeviceLimits(WGPUDevice device) {
    WGPULimits limits = {};
    if (wgpuDeviceGetLimits(device, &limits) == WGPUStatus_Success)
        std::cout << "maxBindGroups: " << limits.maxBindGroups << std::endl;
    return limits;
}
