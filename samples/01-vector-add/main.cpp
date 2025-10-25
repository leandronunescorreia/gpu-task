#include "webgpu_compute/webgpu_compute.h"
#include <iostream>
#include <vector>

int main() {
    DeviceManager dm;
    auto adapter = dm.createAdapter(dm.getInstance());
    auto device = dm.createDevice(adapter);
    dm.queryDeviceLimits(device);

    ComputeEngine engine(device);
    engine.initPipeline("samples/01-vector-add/vec-add.wgsl");

    std::vector<float> input(1024);
    for (size_t i = 0; i < input.size(); ++i)
        input[i] = float(i + 1);

    std::vector<float> output;
    engine.run(input, output);

    for (size_t i = 0; i < 10; ++i)
        std::cout << input[i] << " → " << output[i] << std::endl;

    return 0;
}
