#include <jarcompute/gpu_layout.h>

namespace jarcompute {

void GpuLayout::add(const GpuBinding& b) {
    _bindings.push_back(b);
}

const std::vector<GpuBinding>& GpuLayout::bindings() const {
    return _bindings;
}

} // namespace jarcompute
