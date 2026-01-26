#include "gpu_resource.hpp"

namespace jarcompute {

GpuResource::GpuResource(DeviceContext& ctx, ResourceType type)
    : _ctx(ctx), _type(type) {
    // _rid is default-initialized (invalid)
}

GpuResource::~GpuResource() = default;

} // namespace jarcompute
