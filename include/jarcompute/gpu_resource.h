#ifndef JARCOMPUTE_GPU_RESOURCE_H
#define JARCOMPUTE_GPU_RESOURCE_H

#include "device_context.h"
#include <godot_cpp/classes/rendering_device.hpp>

namespace jarcompute {

class GpuResource {
public:
    enum class ResourceType {
        Buffer,
        Image,
        Sampler,
        CombinedImageSampler,
        SamplerWithTexture,
        External
    };

    virtual ~GpuResource();

    const std::vector<godot::RID>& rids() const { return _rids; }

    godot::RID rid() const { return _rids.empty() ? godot::RID() : _rids[0]; }

    ResourceType resource_type() const { return _type; }

protected:
    GpuResource(DeviceContext& ctx, ResourceType type);
    DeviceContext& _ctx;
    std::vector<godot::RID> _rids;
    ResourceType _type;
};


} // namespace jarcompute

#endif // JARCOMPUTE_GPU_RESOURCE_H
