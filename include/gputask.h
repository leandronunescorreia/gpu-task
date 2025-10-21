#pragma once

/**
 * GPU-Task: Cross-platform GPU Computation Engine
 * 
 * Main header file that includes all necessary components.
 * 
 * Usage:
 *   #include <gputask.h>
 * 
 * Example:
 *   gputask::GPUContext context;
 *   context.initialize();
 *   
 *   gputask::GPUBuffer buffer(context, size, false);
 *   buffer.write(data, size);
 *   
 *   gputask::GPUKernel kernel(context, source, "kernel_name");
 *   kernel.compile();
 *   kernel.setArg(0, buffer);
 *   kernel.execute(work_size);
 *   kernel.finish();
 */

#include "gpu_context.h"
#include "gpu_buffer.h"
#include "gpu_kernel.h"
