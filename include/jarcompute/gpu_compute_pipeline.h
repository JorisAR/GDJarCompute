#ifndef JARCOMPUTE_GPU_COMPUTE_PIPELINE_H
#define JARCOMPUTE_GPU_COMPUTE_PIPELINE_H

#include "device_context.h"
#include "gpu_layout.h"

#include <godot_cpp/classes/rd_shader_file.hpp>
#include <godot_cpp/classes/rd_shader_spirv.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/rd_uniform.hpp>
#include <unordered_map>

namespace jarcompute {

class GpuComputePipeline {
public:
    GpuComputePipeline(DeviceContext& ctx, const godot::String& shader_path);

    void bind_layout(const GpuLayout& layout);
    void set_push_constants(const void* data, size_t bytes);

    void dispatch(uint32_t x, uint32_t y, uint32_t z);

private:
    DeviceContext& _ctx;
    godot::RID _shader;
    godot::RID _pipeline;

    std::vector<uint8_t> _push_constants;

    std::unordered_map<uint32_t, godot::RID> _uniform_sets;
};

} // namespace jarcompute

#endif // JARCOMPUTE_GPU_COMPUTE_PIPELINE_H
