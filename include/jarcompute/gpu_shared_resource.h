#ifndef JARCOMPUTE_GPU_SHARED_RESOURCE_H
#define JARCOMPUTE_GPU_SHARED_RESOURCE_H

#include "gpu_resource.hpp"

namespace jarcompute {

class GpuSharedResource : public GpuResource {
public:
    GpuSharedResource(DeviceContext& ctx,
                      godot::RID external_rid,
                      ResourceType type)
        : GpuResource(ctx, type)
    {
        _rid = external_rid; // no ownership
    }
};

} // namespace jarcompute

#endif // JARCOMPUTE_GPU_SHARED_RESOURCE_H
