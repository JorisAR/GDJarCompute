#include <jarcompute/device_context.h>

using namespace godot;

namespace jarcompute
{

    DeviceContext::DeviceContext(RenderingDevice *existing, bool force_delete_rids)
    {
        if (existing)
        {
            _rd = existing;
            _owns_rd = false;
        }
        else
        {
            _rd = RenderingServer::get_singleton()->create_local_rendering_device();
            _owns_rd = true;
        }

        _force_delete = force_delete_rids;
    }

    DeviceContext::~DeviceContext()
    {
        flush_deletions();
        if (_owns_rd && _rd)
        {
            // Godot owns the RenderingDevice; we don't delete it manually.
            // Just clear the pointer.
            _rd = nullptr;
        }
    }

    RenderingDevice *DeviceContext::rd() const
    {
        return _rd;
    }

    void DeviceContext::submit()
    {
        if (_rd)
        {
            _rd->submit();
        }
    }

    void DeviceContext::sync()
    {
        if (_rd)
        {
            _rd->sync();
        }
    }

    void DeviceContext::queue_free(RID rid)
    {
        if (!rid.is_valid())
            return;
        _deletion_queue.push_back(rid);
    }

    void DeviceContext::force_free(godot::RID rid)
    {
        if (rid.is_valid())
        {
            _rd->free_rid(rid);
        }
    }

    void DeviceContext::free(godot::RID rid)
    {
        if (_force_delete)
            force_free(rid);
        else
            queue_free(rid);
    }

    void DeviceContext::flush_deletions()
    {
        if (!_rd)
            return;

        for (auto it = _deletion_queue.rbegin(); it != _deletion_queue.rend(); ++it)
        {
            if (it->is_valid())
            {
                _rd->free_rid(*it);
            }
        }
        _deletion_queue.clear();
    }

} // namespace jarcompute
