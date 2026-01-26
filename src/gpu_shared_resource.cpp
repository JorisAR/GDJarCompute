#include <jarcompute/gpu_shared_resource.h>

namespace jarcompute
{

GpuSharedResource::GpuSharedResource(DeviceContext &ctx, godot::RID external_rid, ResourceType type)
    : GpuResource(ctx, type)
{
    _rids = {external_rid};
}

GpuSharedResource::GpuSharedResource(DeviceContext &ctx, std::vector<godot::RID> external_rids, ResourceType type)
    : GpuResource(ctx, type)
{
    _rids = external_rids;
}
} // namespace jarcompute
