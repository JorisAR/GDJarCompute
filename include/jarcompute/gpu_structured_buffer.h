#ifndef JARCOMPUTE_GPU_STRUCTURED_BUFFER_H
#define JARCOMPUTE_GPU_STRUCTURED_BUFFER_H

#include "gpu_buffer.h"
#include <type_traits>
#include <span>

namespace jarcompute {

template<typename T>
class GpuStructuredBuffer : public GpuBuffer {
public:
    static_assert(std::is_trivially_copyable_v<T>,
                  "GpuStructuredBuffer<T> requires T to be trivially copyable.");

    using GpuBuffer::GpuBuffer;

    void upload(const T& value) {
        ensure_size(sizeof(T));
        GpuBuffer::upload(&value, sizeof(T));
    }

    void upload(std::span<const T> values) {
        ensure_size(values.size() * sizeof(T));
        GpuBuffer::upload(values.data(), values.size() * sizeof(T));
    }
};

} // namespace jarcompute

#endif // JARCOMPUTE_GPU_STRUCTURED_BUFFER_H
