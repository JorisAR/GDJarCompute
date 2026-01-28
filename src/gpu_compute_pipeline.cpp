#include <jarcompute/gpu_compute_pipeline.h>
#include <godot_cpp/classes/rd_shader_file.hpp>
#include <godot_cpp/classes/rd_shader_spirv.hpp>
#include <godot_cpp/classes/uniform_set_cache_rd.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/rd_uniform.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <map>

using namespace godot;

namespace jarcompute {

GpuComputePipeline::GpuComputePipeline(DeviceContext &ctx, const String &shader_path)
    : _ctx(ctx) {
    RenderingDevice *rd = _ctx.rd();
    _shader_name = shader_path.get_file();
    
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
        UtilityFunctions::printerr("GpuComputePipeline (" + _shader_name + "): Failed to create shader from SPIR-V.");
        return;
    }

    _pipeline = rd->compute_pipeline_create(_shader);
    if (!_pipeline.is_valid()) {
        UtilityFunctions::printerr("GpuComputePipeline (" + _shader_name + "): Failed to create compute pipeline.");
        return;
    }
}

void GpuComputePipeline::bind_layout(const GpuLayout &layout) {
    RenderingDevice *rd = _ctx.rd();
    if (!rd || !_shader.is_valid())
        return;

    _uniform_sets.clear();

    // Group bindings by set index
    std::map<uint32_t, TypedArray<RDUniform>> per_set_uniforms;

    for (const auto &b : layout.bindings()) {
        if (!b.resource)
            continue;

        const auto &ids = b.resource->rids();
        if (ids.empty())
            continue;

        Ref<RDUniform> u;
        u.instantiate();
        u->set_binding(b.binding);
        u->set_uniform_type(b.uniform_type);

        for (const RID &rid : ids) {
            if (rid.is_valid())
                u->add_id(rid);
        }

        // If no valid IDs ended up in the uniform, skip it
        if (u->get_ids().is_empty())
            continue;

        per_set_uniforms[b.set].push_back(u);
    }

    // Create uniform sets only for sets we actually populated
    for (auto &[set_index, uniforms] : per_set_uniforms) {
        if (uniforms.is_empty())
            continue;

        // RID set_rid = rd->uniform_set_create(uniforms, _shader, set_index);
        RID set_rid = UniformSetCacheRD::get_cache(_shader, set_index, uniforms);
        if (!set_rid.is_valid()) {
            UtilityFunctions::printerr("GpuComputePipeline (" + _shader_name + "): Failed to create uniform set for set ", set_index);
            continue;
        }
        _uniform_sets[set_index] = set_rid;
    }
}

void GpuComputePipeline::set_push_constants(const void *data, size_t bytes) {
    _push_constants.resize(bytes);
    if (bytes > 0 && data) {
        std::memcpy(_push_constants.data(), data, bytes);
    }
}

void GpuComputePipeline::dispatch(uint32_t x, uint32_t y, uint32_t z) {
    RenderingDevice *rd = _ctx.rd();
    if (!rd || !_pipeline.is_valid())
        return;

    int32_t list = rd->compute_list_begin();
    rd->compute_list_bind_compute_pipeline(list, _pipeline);

    // Bind uniform sets
    for (const auto &[set_index, set_rid] : _uniform_sets) {
        if (set_rid.is_valid()) {
            rd->compute_list_bind_uniform_set(list, set_rid, set_index);
        }
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
