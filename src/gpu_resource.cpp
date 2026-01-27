#include <jarcompute/gpu_resource.h>

namespace jarcompute {

GpuResource::GpuResource(DeviceContext& ctx, ResourceType type)
    : _ctx(ctx), _type(type) {
    // _rid is default-initialized (invalid)
}

GpuResource::~GpuResource() {
    for (const godot::RID& rid : _rids) {
        _ctx.queue_free(rid);
    }
}

} // namespace jarcompute
