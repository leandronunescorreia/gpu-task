#include "webgpu_compute/webgpu_compute.h"
#include <iostream>
#include <vector>

int main()
{
    DeviceManager dm;
    auto adapter = dm.createAdapter(dm.getInstance());
    auto device = dm.createDevice(adapter);
    auto limits = dm.queryDeviceLimits(device);

    auto inputBuffer = BufferManager::createStorageBuffer(device, 1024 * sizeof(float), true);
    auto outputBuffer = BufferManager::createStorageBuffer(device, 1024 * sizeof(float), false);

    ComputeEngine engine(device);
    auto vectorAddPipeline = engine.initPipeline("samples/01-vector-add/vec-add.wgsl",
                                                 {inputBuffer}, {outputBuffer}, 1024 * sizeof(float));

    auto effectStore = ComputeEffectStore();
    effectStore.addEffect("vectorAdd", std::make_unique<ComputeEffect>(vectorAddPipeline));

    std::vector<float> input(1024);
    for (size_t i = 0; i < input.size(); ++i)
        input[i] = float(i + 1);

    std::vector<float> output;
    auto fx = effectStore.get("vectorAdd");

    engine.run({inputBuffer}, {outputBuffer}, {*fx});

    for (size_t i = 0; i < 10; ++i)
        std::cout << input[i] << " → " << output[i] << std::endl;

    return 0;
}
