#include "gpu_shared_resource.hpp"

namespace jarcompute {

GpuSharedResource::GpuSharedResource(DeviceContext& ctx,
                                     godot::RID external_rid,
                                     ResourceType type)
    : GpuResource(ctx, type) {
    _rid = external_rid; // no ownership
}

} // namespace jarcompute
