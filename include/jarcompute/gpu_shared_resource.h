#ifndef JARCOMPUTE_GPU_SHARED_RESOURCE_H
#define JARCOMPUTE_GPU_SHARED_RESOURCE_H

#include "gpu_resource.h"

namespace jarcompute
{

class GpuSharedResource : public GpuResource
{
  public:
    GpuSharedResource(DeviceContext &ctx, godot::RID external_rid, ResourceType type);

    GpuSharedResource(DeviceContext &ctx, std::vector<godot::RID> external_rids, ResourceType type);
};

} // namespace jarcompute

#endif // JARCOMPUTE_GPU_SHARED_RESOURCE_H
