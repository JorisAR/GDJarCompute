#ifndef JARCOMPUTE_DEVICE_CONTEXT_H
#define JARCOMPUTE_DEVICE_CONTEXT_H

#include <godot_cpp/classes/rendering_device.hpp>
#include <godot_cpp/classes/rendering_server.hpp>
#include <vector>

namespace jarcompute {

class DeviceContext {
public:
    DeviceContext(godot::RenderingDevice* existing = nullptr);
    ~DeviceContext();

    godot::RenderingDevice* rd() const;

    void submit();
    void sync();

    void free_later(godot::RID rid);
    void flush_deletions();

private:
    godot::RenderingDevice* _rd = nullptr;
    bool _owns_rd = false;

    std::vector<godot::RID> _deletion_queue;
};

} // namespace jarcompute

#endif // JARCOMPUTE_DEVICE_CONTEXT_H
