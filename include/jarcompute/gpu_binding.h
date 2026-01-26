#ifndef JARCOMPUTE_GPU_BINDING_H
#define JARCOMPUTE_GPU_BINDING_H

#include "gpu_resource.h"
#include <godot_cpp/classes/rendering_device.hpp>

namespace jarcompute {

struct GpuBinding {
    uint32_t set = 0;
    uint32_t binding = 0;
    godot::RenderingDevice::UniformType uniform_type;
    GpuResource* resource = nullptr;

    // Constructor using default uniform type
    GpuBinding(uint32_t s, uint32_t b, GpuResource& res)
        : set(s), binding(b),
          uniform_type(default_uniform_type_for(res)),
          resource(&res) {}

    // Constructor with explicit override
    GpuBinding(uint32_t s, uint32_t b,
               godot::RenderingDevice::UniformType override_type,
               GpuResource& res)
        : set(s), binding(b),
          uniform_type(override_type),
          resource(&res) {}

private:
    static godot::RenderingDevice::UniformType
    default_uniform_type_for(const GpuResource& res);
};

} // namespace jarcompute

#endif // JARCOMPUTE_GPU_BINDING_H
