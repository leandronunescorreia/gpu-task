# gpu-task
cross-platform gpu computation engine


# initial cmake config command

```bash
cmake -S . -B build -DBUILD_SAMPLES=OFF
```

# initial build command
```bash
cmake --build build --target webgpu_compute -- -j$(nproc)
```


# for install build

> configure
```bash
cmake -S . -B build -DBUILD_SAMPLES=OFF -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=install
```
> build
# initial build command
```bash
cmake --build build --target webgpu_compute -- -j$(nproc)
```

> install
```bash
cmake --install build
```