#include "webgpu_compute/device_manager.h"
#include <iostream>
#include <cstring>
#include <stdexcept>

DeviceManager::DeviceManager() {
    WGPUInstanceDescriptor desc = {};
    desc.nextInChain = nullptr;

#ifdef WEBGPU_BACKEND_EMSCRIPTEN
    this->m_instance = wgpuCreateInstance(nullptr);
#else //  WEBGPU_BACKEND_EMSCRIPTEN
    this->m_instance = wgpuCreateInstance(&desc);
#endif //  WEBGPU_BACKEND_EMSCRIPTEN

    if (!this->m_instance) {
        std::cerr << "Could not initialize WebGPU!" << std::endl;
        throw std::runtime_error("Could not initialize WebGPU instance");
    }

#if defined(DEBUG) || defined(_DEBUG) || !defined(NDEBUG)
    std::cout << "WebGPU instance " << std::hex << this->m_instance << std::dec
              << " created successfully." << std::endl;
#endif
}

DeviceManager::~DeviceManager() {
    if (m_device) wgpuDeviceRelease(m_device);
    if (m_instance) wgpuInstanceRelease(m_instance);
}

WGPUAdapter DeviceManager::createAdapter(WGPUInstance instance) {
	WGPURequestAdapterOptions adapterOpts = {};
	adapterOpts.nextInChain = nullptr;
    adapterOpts.backendType = WGPUBackendType_Vulkan;  // or Metal / D3D12 / OpenGL
	WGPUAdapter adapter = {};

	auto onAdapterRequestEnded = [](WGPURequestAdapterStatus status, WGPUAdapter adapter, WGPUStringView message, WGPU_NULLABLE void* userdata1, WGPU_NULLABLE void* userdata2) {
        (void) userdata2;
		if (status == WGPURequestAdapterStatus_Success) {
            WGPUAdapter& userData = *reinterpret_cast<WGPUAdapter*>(userdata1);
			userData = adapter;
		}
	};

    WGPURequestAdapterCallbackInfo callbackInfo = {};
    callbackInfo.nextInChain = nullptr;
    callbackInfo.mode = WGPUCallbackMode_AllowSpontaneous;
    callbackInfo.callback = onAdapterRequestEnded;
    callbackInfo.userdata1 = (void*)&adapter;

	wgpuInstanceRequestAdapter(
		this->m_instance,
		&adapterOpts,
        callbackInfo
	);

    if (!adapter) {
        std::cerr << "Could not initialize the Adapter!" << std::endl;
        throw std::runtime_error("Could not initialize WebGPU adapter");
    }

#if defined(DEBUG) || defined(_DEBUG) || !defined(NDEBUG)
	std::cout << "Got adapter: " << adapter << std::endl;
#endif

    return adapter;
}

WGPUDevice DeviceManager::createDevice(WGPUAdapter adapter) {
    WGPUDeviceDescriptor deviceDesc = {};
    deviceDesc.nextInChain = nullptr;
    const char* deviceLabel = "My Device";
    const char* defaultQueueLabel = "The default queue";
    deviceDesc.label = WGPUStringView{deviceLabel, static_cast<uint32_t>(std::strlen(deviceLabel))};
    deviceDesc.requiredFeatureCount = 0; // we do not require any specific feature
	deviceDesc.requiredLimits = nullptr; // we do not require any specific limit
    deviceDesc.defaultQueue.nextInChain = nullptr;
    deviceDesc.defaultQueue.label = WGPUStringView{defaultQueueLabel, static_cast<uint32_t>(std::strlen(defaultQueueLabel))};


    auto deviceLostCallback = [](WGPUDevice const * device, WGPUDeviceLostReason reason, WGPUStringView message, WGPU_NULLABLE void* userdata1, WGPU_NULLABLE void* userdata2) {
        std::cout << "Device " << std::hex << device << std::dec << " lost callback invoked, reason: " << std::hex << reason << std::dec;
        std::cout << ", message: " << std::string(message.data, message.length);
        if(userdata1 || userdata2) {
            std::cout << ", userdata not null!";
        }
        std::cout << std::endl;
    };

    WGPUDeviceLostCallbackInfo deviceLostCallbackInfo = {};
    deviceLostCallbackInfo.nextInChain = nullptr;
    deviceLostCallbackInfo.mode = WGPUCallbackMode_AllowSpontaneous;
    deviceLostCallbackInfo.callback = deviceLostCallback;
    deviceLostCallbackInfo.userdata1 = nullptr;
    deviceLostCallbackInfo.userdata2 = nullptr;

    auto uncapturedErrorCallback = [](WGPUDevice const * device, WGPUErrorType type, WGPUStringView message, WGPU_NULLABLE void* userdata1, WGPU_NULLABLE void* userdata2) {
        std::cout << "Device " << std::hex << device << std::dec << " uncaptured error callback invoked, type: " << std::hex << type << std::dec;
        std::cout << ", message: " << std::string(message.data, message.length);
        if(userdata1 || userdata2) {
            std::cout << ", userdata not null!";
        }
        std::cout << std::endl;
    };
    WGPUUncapturedErrorCallbackInfo uncapturedErrorCallbackInfo = {};
    uncapturedErrorCallbackInfo.nextInChain = nullptr;
    uncapturedErrorCallbackInfo.callback = uncapturedErrorCallback;
    uncapturedErrorCallbackInfo.userdata1 = nullptr;
    uncapturedErrorCallbackInfo.userdata2 = nullptr;

    deviceDesc.deviceLostCallbackInfo = deviceLostCallbackInfo;
    deviceDesc.uncapturedErrorCallbackInfo = uncapturedErrorCallbackInfo;

	// A function that is invoked whenever the device stops being available.
	auto onDeviceRequestEnded = [](WGPURequestDeviceStatus status, WGPUDevice device, WGPUStringView message, WGPU_NULLABLE void* userdata1, WGPU_NULLABLE void* userdata2) {
		WGPUDevice& userData = *reinterpret_cast<WGPUDevice*>(userdata1);
		if (status == WGPURequestDeviceStatus_Success) {
			userData = device;
		} else {
			std::cout << "Could not get WebGPU device: " << std::string(message.data, message.length) << std::endl;
		}
        if(userdata2 || message.length > 0) {
            std::cout << "Userdata2 is not null!" << std::endl;
        }        
	};


    WGPURequestDeviceCallbackInfo callbackInfoDevice = {};
    callbackInfoDevice.nextInChain = nullptr;
    callbackInfoDevice.mode = WGPUCallbackMode_AllowSpontaneous;
    callbackInfoDevice.callback = onDeviceRequestEnded;
    callbackInfoDevice.userdata1 = (void*)&this->m_device;
    callbackInfoDevice.userdata2 = nullptr;
	wgpuAdapterRequestDevice(
		adapter,
		&deviceDesc,
		callbackInfoDevice
	);

    if (!this->m_device) {
        std::cerr << "Could not initialize the Device!" << std::endl;
        throw std::runtime_error("Could not initialize WebGPU device");
    }

    std::cout << "WebGPU device " << std::hex << this->m_device << std::dec << " created successfully." << std::endl;

    wgpuAdapterRelease(adapter);

    return m_device;
}

WGPULimits DeviceManager::queryDeviceLimits(WGPUDevice device) {
    WGPUSupportedFeatures* supported = new WGPUSupportedFeatures();
	wgpuDeviceGetFeatures(device, supported);

	std::cout << "Device features:" << std::endl;
	std::cout << std::hex;
	for (size_t i = 0; i < supported->featureCount; ++i) {
		auto f = supported->features[i];
        std::cout << std::hex;
		std::cout << " - 0x" << f << std::endl;
	}
	std::cout << std::dec;

	WGPULimits limits = {};
	limits.nextInChain = nullptr;
    
    WGPUStatus status = WGPUStatus_Error;
#ifdef WEBGPU_BACKEND_DAWN
	status = wgpuDeviceGetLimits(device, &limits);
#else
	status = wgpuDeviceGetLimits(device, &limits);
#endif
	
	if (status == WGPUStatus_Success) {
		std::cout << "Device limits:" << std::endl;
		std::cout << " - maxTextureDimension1D: " << limits.maxTextureDimension1D << std::endl;
		std::cout << " - maxTextureDimension2D: " << limits.maxTextureDimension2D << std::endl;
		std::cout << " - maxTextureDimension3D: " << limits.maxTextureDimension3D << std::endl;
		std::cout << " - maxTextureArrayLayers: " << limits.maxTextureArrayLayers << std::endl;
		std::cout << " - maxBindGroups: " << limits.maxBindGroups << std::endl;
		std::cout << " - maxDynamicUniformBuffersPerPipelineLayout: " << limits.maxDynamicUniformBuffersPerPipelineLayout << std::endl;
		std::cout << " - maxDynamicStorageBuffersPerPipelineLayout: " << limits.maxDynamicStorageBuffersPerPipelineLayout << std::endl;
		std::cout << " - maxSampledTexturesPerShaderStage: " << limits.maxSampledTexturesPerShaderStage << std::endl;
		std::cout << " - maxSamplersPerShaderStage: " << limits.maxSamplersPerShaderStage << std::endl;
		std::cout << " - maxStorageBuffersPerShaderStage: " << limits.maxStorageBuffersPerShaderStage << std::endl;
		std::cout << " - maxStorageTexturesPerShaderStage: " << limits.maxStorageTexturesPerShaderStage << std::endl;
		std::cout << " - maxUniformBuffersPerShaderStage: " << limits.maxUniformBuffersPerShaderStage << std::endl;
		std::cout << " - maxUniformBufferBindingSize: " << limits.maxUniformBufferBindingSize << std::endl;
		std::cout << " - maxStorageBufferBindingSize: " << limits.maxStorageBufferBindingSize << std::endl;
		std::cout << " - minUniformBufferOffsetAlignment: " << limits.minUniformBufferOffsetAlignment << std::endl;
		std::cout << " - minStorageBufferOffsetAlignment: " << limits.minStorageBufferOffsetAlignment << std::endl;
		std::cout << " - maxVertexBuffers: " << limits.maxVertexBuffers << std::endl;
		std::cout << " - maxVertexAttributes: " << limits.maxVertexAttributes << std::endl;
		std::cout << " - maxVertexBufferArrayStride: " << limits.maxVertexBufferArrayStride << std::endl;
		std::cout << " - maxInterStageShaderComponents: " << limits.maxInterStageShaderVariables << std::endl;
		std::cout << " - maxComputeWorkgroupStorageSize: " << limits.maxComputeWorkgroupStorageSize << std::endl;
		std::cout << " - maxComputeInvocationsPerWorkgroup: " << limits.maxComputeInvocationsPerWorkgroup << std::endl;
		std::cout << " - maxComputeWorkgroupSizeX: " << limits.maxComputeWorkgroupSizeX << std::endl;
		std::cout << " - maxComputeWorkgroupSizeY: " << limits.maxComputeWorkgroupSizeY << std::endl;
		std::cout << " - maxComputeWorkgroupSizeZ: " << limits.maxComputeWorkgroupSizeZ << std::endl;
		std::cout << " - maxComputeWorkgroupsPerDimension: " << limits.maxComputeWorkgroupsPerDimension << std::endl;
	}

    return limits;
}
