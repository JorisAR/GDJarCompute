# JarCompute

JarCompute is a lightweight C++ library that makes working with Godot’s `RenderingDevice` compute shaders easier, safer, and more expressive.

It provides:

- A unified GPU resource model (`GpuResource`, `GpuBuffer`, `GpuImage`, etc.)
- Automatic buffer resizing and upload helpers
- Clean shader binding through `GpuBinding` and `GpuLayout`
- A simple `GpuComputePipeline` wrapper for dispatching compute workloads
- Utilities for struct packing, push constant alignment, and byte conversion

JarCompute is designed to be used inside a Godot 4 GDExtension, but the core logic is engine‑agnostic.

---

## Example Usage

Below is a minimal example showing how to upload data, bind resources, and dispatch a compute shader.

```cpp

struct Params {
    float time;
    float scale;
    float padding[2]; // ensure 16-byte alignment
};

void run_compute(RenderingDevice* rd) {
    // Create a device context (does not take ownership of rd)
    DeviceContext ctx(rd);

    // Create a structured buffer for parameters
    GpuStructuredBuffer<Params> param_buffer(ctx);
    Params p { 1.0f, 2.0f };
    param_buffer.upload(p);

    // Create a raw storage buffer
    GpuBuffer output_buffer(ctx, 1024);

    // Bind resources to shader layout
    GpuLayout layout;
    layout.add({0, 0, param_buffer});   // default: storage buffer
    layout.add({0, 1, output_buffer});  // default: storage buffer

    // Load compute shader and bind layout
    GpuComputePipeline pipeline(ctx, "res://compute/my_shader.glsl");
    pipeline.bind_layout(layout);

    // Optional push constants
    struct PushData { float dt; float pad[3]; };
    PushData push { 0.016f };
    auto pc = pack_push_constants(push);
    pipeline.set_push_constants(pc.ptr(), pc.size());

    // Dispatch compute shader
    pipeline.dispatch(32, 1, 1);

    // Submit and sync GPU work
    ctx.submit();
    ctx.sync();
}
```

---

## Project Structure

```
JarCompute/
  include/
    jarcompute/
      *.h        # Public headers
  src/
    *.cpp        # Implementations
  README.md
  LICENSE
```

---

## Building

JarCompute is intended to be built as part of a Godot GDExtension project.  
Add the `include/` directory to your include paths and compile the `.cpp` files into your extension. It is recommended to put the root of this project in the `src/` directory of your GDExtension repository.

---

## Contributing

Contributions are welcome!  
Please fork the repository and submit a pull request.

---

## License

This project is licensed under the MIT License.  
See the [LICENSE] file for details.
