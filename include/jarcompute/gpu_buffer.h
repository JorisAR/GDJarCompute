#ifndef JARCOMPUTE_GPU_BUFFER_H
#define JARCOMPUTE_GPU_BUFFER_H

#include "gpu_resource.h"
#include <cstddef>

namespace jarcompute {

class GpuBuffer : public GpuResource {
public:
    GpuBuffer(DeviceContext& ctx, size_t initial_size = 0);

    void ensure_size(size_t bytes);
    void upload(const void* data, size_t bytes, size_t offset = 0);
    void download(void* dst, size_t bytes, size_t offset = 0);

    size_t capacity() const { return _capacity; }

protected:
    size_t _capacity = 0;
};

} // namespace jarcompute

#endif // JARCOMPUTE_GPU_BUFFER_H
