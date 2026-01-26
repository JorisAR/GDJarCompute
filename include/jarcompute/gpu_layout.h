#ifndef JARCOMPUTE_GPU_LAYOUT_H
#define JARCOMPUTE_GPU_LAYOUT_H

#include "gpu_binding.h"
#include <vector>

namespace jarcompute {

class GpuLayout {
public:
    void add(const GpuBinding& b);

    const std::vector<GpuBinding>& bindings() const;

private:
    std::vector<GpuBinding> _bindings;
};

} // namespace jarcompute

#endif // JARCOMPUTE_GPU_LAYOUT_H
