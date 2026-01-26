#include "gpu_binding.hpp"
#include <godot_cpp/classes/rendering_device.hpp>

using namespace godot;

namespace jarcompute {

RenderingDevice::UniformType
GpuBinding::default_uniform_type_for(const GpuResource& res) {
    switch (res.resource_type()) {
        case GpuResource::ResourceType::Buffer:
            return RenderingDevice::UNIFORM_TYPE_STORAGE_BUFFER;
        case GpuResource::ResourceType::Image:
            return RenderingDevice::UNIFORM_TYPE_IMAGE;
        case GpuResource::ResourceType::Sampler:
            return RenderingDevice::UNIFORM_TYPE_SAMPLER;
        case GpuResource::ResourceType::CombinedImageSampler:
            return RenderingDevice::UNIFORM_TYPE_SAMPLER_WITH_TEXTURE;
        case GpuResource::ResourceType::External:
        default:
            // External is ambiguous; default to storage buffer as a safe-ish fallback.
            return RenderingDevice::UNIFORM_TYPE_STORAGE_BUFFER;
    }
}

} // namespace jarcompute
