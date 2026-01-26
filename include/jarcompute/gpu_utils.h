#ifndef JARCOMPUTE_GPU_UTILS_H
#define JARCOMPUTE_GPU_UTILS_H

#include <godot_cpp/variant/packed_byte_array.hpp>
#include <cstdint>
#include <cstring>
#include <type_traits>
#include <vector>

namespace jarcompute {

// ------------------------------------------------------------
//  Alignment helpers
// ------------------------------------------------------------

inline constexpr size_t align_up(size_t value, size_t alignment) {
    return (value + alignment - 1) & ~(alignment - 1);
}

// ------------------------------------------------------------
//  Convert a trivially copyable struct to PackedByteArray
// ------------------------------------------------------------

template <typename T>
inline godot::PackedByteArray struct_to_bytes(const T& obj) {
    static_assert(std::is_trivially_copyable_v<T>,
                  "struct_to_bytes<T> requires T to be trivially copyable.");

    godot::PackedByteArray arr;
    arr.resize(sizeof(T));
    std::memcpy(arr.ptrw(), &obj, sizeof(T));
    return arr;
}

// ------------------------------------------------------------
//  Pack push constants (must be 16-byte aligned)
// ------------------------------------------------------------

template <typename T>
inline godot::PackedByteArray pack_push_constants(const T& obj) {
    static_assert(std::is_trivially_copyable_v<T>,
                  "pack_push_constants<T> requires T to be trivially copyable.");

    constexpr size_t raw_size = sizeof(T);
    constexpr size_t aligned_size = align_up(raw_size, 16);

    godot::PackedByteArray arr;
    arr.resize(aligned_size);

    // Copy struct
    std::memcpy(arr.ptrw(), &obj, raw_size);

    // Zero padding
    if (aligned_size > raw_size) {
        std::memset(arr.ptrw() + raw_size, 0, aligned_size - raw_size);
    }

    return arr;
}

} // namespace jarcompute

#endif // JARCOMPUTE_GPU_UTILS_H
