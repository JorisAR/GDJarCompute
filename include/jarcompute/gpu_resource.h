#ifndef JARCOMPUTE_GPU_RESOURCE_H
#define JARCOMPUTE_GPU_RESOURCE_H

#include "device_context.hpp"
#include <godot_cpp/classes/rendering_device.hpp>

namespace jarcompute {

class GpuResource {
public:
    enum class ResourceType {
        Buffer,
        Image,
        Sampler,
        CombinedImageSampler,
        External
    };

    virtual ~GpuResource() = default;

    godot::RID rid() const { return _rid; }
    ResourceType resource_type() const { return _type; }

protected:
    GpuResource(DeviceContext& ctx, ResourceType type)
        : _ctx(ctx), _type(type) {}

    DeviceContext& _ctx;
    godot::RID _rid;
    ResourceType _type;
};

} // namespace jarcompute

#endif // JARCOMPUTE_GPU_RESOURCE_H
