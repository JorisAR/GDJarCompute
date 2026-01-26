#ifndef JARCOMPUTE_GPU_LAYOUT_H
#define JARCOMPUTE_GPU_LAYOUT_H

#include "gpu_binding.hpp"
#include <vector>

namespace jarcompute {

class GpuLayout {
public:
    void add(const GpuBinding& b) {
        _bindings.push_back(b);
    }

    const std::vector<GpuBinding>& bindings() const {
        return _bindings;
    }

private:
    std::vector<GpuBinding> _bindings;
};

} // namespace jarcompute

#endif // JARCOMPUTE_GPU_LAYOUT_H
