#ifndef JARCOMPUTE_DEVICE_CONTEXT_H
#define JARCOMPUTE_DEVICE_CONTEXT_H

#include <godot_cpp/classes/rendering_device.hpp>
#include <godot_cpp/classes/rendering_server.hpp>
#include <vector>

namespace jarcompute {

class DeviceContext {
public:
    DeviceContext(godot::RenderingDevice* existing = nullptr, bool force_delete_rids = true);
    ~DeviceContext();

    godot::RenderingDevice* rd() const;

    void submit();
    void sync();

    void queue_free(godot::RID rid); // Queue free when flush deletions is called
    void force_free(godot::RID rid); //Delete immediately
    void free(godot::RID rid); // Let settings decide
    void flush_deletions();

private:
    godot::RenderingDevice* _rd = nullptr;
    bool _owns_rd = false;
    bool _force_delete = true;

    std::vector<godot::RID> _deletion_queue;
};

} // namespace jarcompute

#endif // JARCOMPUTE_DEVICE_CONTEXT_H
