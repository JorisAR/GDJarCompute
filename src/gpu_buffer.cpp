#include "gpu_buffer.hpp"
#include <godot_cpp/variant/packed_byte_array.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <cstring>

using namespace godot;

namespace jarcompute {

GpuBuffer::GpuBuffer(DeviceContext& ctx, size_t initial_size)
    : GpuResource(ctx, ResourceType::Buffer) {
    if (initial_size > 0) {
        PackedByteArray init;
        init.resize(initial_size);
        memset(init.ptrw(), 0, initial_size);
        _rid = _ctx.rd()->storage_buffer_create(initial_size, init);
        _capacity = initial_size;
    }
}

void GpuBuffer::ensure_size(size_t bytes) {
    if (bytes == 0)
        bytes = 1; // avoid zero-sized buffers

    if (bytes <= _capacity && _rid.is_valid())
        return;

    // Recreate buffer
    if (_rid.is_valid()) {
        _ctx.free_later(_rid);
        _rid = RID();
    }

    PackedByteArray init;
    init.resize(bytes);
    memset(init.ptrw(), 0, bytes);

    _rid = _ctx.rd()->storage_buffer_create(bytes, init);
    _capacity = bytes;
}

void GpuBuffer::upload(const void* data, size_t bytes, size_t offset) {
    if (!_ctx.rd())
        return;

    if (!_rid.is_valid() || _capacity < offset + bytes) {
        ensure_size(offset + bytes);
    }

    PackedByteArray tmp;
    tmp.resize(bytes);
    std::memcpy(tmp.ptrw(), data, bytes);

    _ctx.rd()->buffer_update(_rid, offset, bytes, tmp);
}

void GpuBuffer::download(void* dst, size_t bytes, size_t offset) {
    if (!_ctx.rd() || !_rid.is_valid())
        return;

    PackedByteArray data = _ctx.rd()->buffer_get_data(_rid);
    if (data.size() < static_cast<int>(offset + bytes))
        bytes = data.size() > static_cast<int>(offset) ? data.size() - offset : 0;

    if (bytes > 0) {
        std::memcpy(dst, data.ptr() + offset, bytes);
    }
}

} // namespace jarcompute
