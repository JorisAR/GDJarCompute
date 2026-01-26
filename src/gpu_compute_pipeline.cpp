#include "gpu_compute_pipeline.hpp"
#include <godot_cpp/classes/rd_shader_file.hpp>
#include <godot_cpp/classes/rd_shader_spirv.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/rd_uniform.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

namespace jarcompute {

GpuComputePipeline::GpuComputePipeline(DeviceContext& ctx, const String& shader_path)
    : _ctx(ctx) {
    RenderingDevice* rd = _ctx.rd();
    if (!rd) {
        UtilityFunctions::printerr("GpuComputePipeline: RenderingDevice is null.");
        return;
    }

    Ref<RDShaderFile> shader_file = ResourceLoader::get_singleton()->load(shader_path);
    if (shader_file.is_null()) {
        UtilityFunctions::printerr("GpuComputePipeline: Failed to load shader file: ", shader_path);
        return;
    }

    Ref<RDShaderSPIRV> spirv = shader_file->get_spirv();
    if (spirv.is_null()) {
        UtilityFunctions::printerr("GpuComputePipeline: Failed to get SPIR-V from shader file: ", shader_path);
        return;
    }

    _shader = rd->shader_create_from_spirv(spirv);
    if (!_shader.is_valid()) {
        UtilityFunctions::printerr("GpuComputePipeline: Failed to create shader from SPIR-V.");
        return;
    }

    _pipeline = rd->compute_pipeline_create(_shader);
    if (!_pipeline.is_valid()) {
        UtilityFunctions::printerr("GpuComputePipeline: Failed to create compute pipeline.");
        return;
    }
}

void GpuComputePipeline::bind_layout(const GpuLayout& layout) {
    RenderingDevice* rd = _ctx.rd();
    if (!rd || !_shader.is_valid())
        return;

    _uniform_sets.clear();

    // Group bindings by set
    std::unordered_map<uint32_t, TypedArray<RDUniform>> per_set_uniforms;

    for (const auto& b : layout.bindings()) {
        if (!b.resource || !b.resource->rid().is_valid())
            continue;

        Ref<RDUniform> u;
        u.instantiate();
        u->set_binding(b.binding);
        u->set_uniform_type(b.uniform_type);
        u->add_id(b.resource->rid());

        per_set_uniforms[b.set].push_back(u);
    }

    // Create uniform sets
    for (auto& [set_index, uniforms] : per_set_uniforms) {
        RID set_rid = rd->uniform_set_create(uniforms, _shader, set_index);
        _uniform_sets[set_index] = set_rid;
    }
}

void GpuComputePipeline::set_push_constants(const void* data, size_t bytes) {
    _push_constants.resize(bytes);
    if (bytes > 0 && data) {
        std::memcpy(_push_constants.data(), data, bytes);
    }
}

void GpuComputePipeline::dispatch(uint32_t x, uint32_t y, uint32_t z) {
    RenderingDevice* rd = _ctx.rd();
    if (!rd || !_pipeline.is_valid())
        return;

    int32_t list = rd->compute_list_begin();
    rd->compute_list_bind_compute_pipeline(list, _pipeline);

    // Bind uniform sets
    for (const auto& [set_index, set_rid] : _uniform_sets) {
        rd->compute_list_bind_uniform_set(list, set_rid, set_index);
    }

    // Push constants (if any)
    if (!_push_constants.empty()) {
        PackedByteArray pc;
        pc.resize(_push_constants.size());
        std::memcpy(pc.ptrw(), _push_constants.data(), _push_constants.size());
        rd->compute_list_set_push_constant(list, pc, pc.size());
    }

    rd->compute_list_dispatch(list, x, y, z);
    rd->compute_list_end();
}

} // namespace jarcompute
