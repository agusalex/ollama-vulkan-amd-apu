<div align="center">
  <h2>Ollama Vulkan (AMD/APU)</h2>
  <a href="https://github.com/users/agusalex/packages/container/package/ollama-vulkan-amd-apu">
    <img alt="ghcr image" src="https://img.shields.io/badge/ghcr.io-agusalex%2Follama--vulkan--amd--apu-2a2f3a">
  </a>
</div>

### What’s different from upstream

This fork adds a Vulkan backend to enable GPU acceleration on AMD GPUs and APUs. It keeps Ollama’s UX and APIs the same, but changes discovery, build, and runtime to support Vulkan.

- **Vulkan backend for GGML**: new `ggml-vulkan` target, SPIR-V shader generation/embedding, and CMake `Vulkan` preset.
- **Vulkan GPU discovery**: detects Vulkan devices, reports VRAM (using `VK_EXT_memory_budget` when available), and integrates into the refresh loop.
- **AMD-focused handling**: vendor/driver awareness and heuristics for AMD.
- **Packaging**: final image contains Vulkan runtime libraries and shaders; no change to CLI/API usage.

### Prebuilt container

- GHCR image: `ghcr.io/agusalex/ollama-vulkan-amd-apu`

Pull:

```bash
docker pull ghcr.io/agusalex/ollama-vulkan-amd-apu:latest
```

Run (Linux with AMD GPU, Vulkan via DRM):

```bash
docker run --rm -it \
  --device /dev/dri \
  -e GGML_VK_VISIBLE_DEVICES=0 \
  -p 11434:11434 \
  ghcr.io/agusalex/ollama-vulkan-amd-apu:latest
```

Notes:
- Exposes the server on `0.0.0.0:11434` (same as upstream). Use your usual client tools.
- Ensure the host has an AMD GPU with up-to-date Vulkan drivers. On Linux, `/dev/dri` must be present and accessible.

### Environment variables (additions)

- `GGML_VK_VISIBLE_DEVICES`: comma-separated Vulkan device indices to expose (e.g., `0` or `0,1`).
- Optional Vulkan debug/checks (advanced):
  - `GGML_VULKAN_SKIP_CHECKS`: skip internal results checking.
  - `GGML_VULKAN_OUTPUT_TENSOR`: enable output tensor dumps for validation.

Upstream AMD/HIP variables (still available when using HIP/ROCm paths):
- `HIP_VISIBLE_DEVICES`, `ROCR_VISIBLE_DEVICES`, `GPU_DEVICE_ORDINAL`.

### Build locally (Vulkan component)

Requirements: CMake ≥ 3.19, C++17 compiler, Vulkan SDK (headers + `glslc`).

```bash
cmake --preset Vulkan
cmake --build --parallel --preset Vulkan
cmake --install build --component Vulkan --strip --parallel 8
```

### Limitations

- Requires Vulkan 1.2+ capable drivers on the host.
- Cooperative matrix acceleration depends on device/driver support.

### Links

- Repository: `https://github.com/agusalex/ollama-vulkan-amd-apu`
- Container: `ghcr.io/agusalex/ollama-vulkan-amd-apu`


