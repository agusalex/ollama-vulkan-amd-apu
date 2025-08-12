### Ollama on Vulkan (AMD/APU)
[![ghcr](https://img.shields.io/badge/ghcr.io-agusalex%2Follama-vulkan-amd-apu-2f5dff?logo=github)](https://github.com/users/agusalex/packages/container/package/ollama-vulkan-amd-apu:latest)

This fork adds a Vulkan backend so Ollama can run on AMD GPUs and APUs. It keeps the CLI and API the same, but swaps in Vulkan where available.
It was tested on an AMD Ryzen 7 5700U with Radeon Graphics @ 1800 MHz Lucienne iGU


### What changed

- ggml Vulkan backend wired into the build (CMake preset/targets for `ggml-vulkan`)
- Vulkan shader toolchain and SPIR-V generation embedded in the build
- Vulkan-based GPU discovery and VRAM reporting
- AMD-oriented handling and heuristics in device selection
- Container/Docker support that installs Vulkan runtime/headers needed for the backend

### Try these runtime flags

Set the following environment variables to force the Vulkan path and typical defaults:

```bash
OLLAMA_FLASH_ATTENTION=0
OLLAMA_KV_CACHE_TYPE=f16
OLLAMA_LLM_LIBRARY=vulkan
OLLAMA_MAX_VRAM=10000000000 # Play with this one until it matches your igpus vram usage
```

Example (Docker):

```bash
docker run --rm -it \
  --device /dev/dri \
  -e OLLAMA_FLASH_ATTENTION=0 \
  -e OLLAMA_KV_CACHE_TYPE=f16 \
  -e OLLAMA_LLM_LIBRARY=vulkan \
  -e OLLAMA_MAX_VRAM=10000000000 \
  -p 11434:11434 \
  ghcr.io/agusalex/ollama-vulkan-amd-apu:latest
```

Notes:
- Requires a Vulkan 1.2+ capable AMD GPU/APU and drivers installed on the host.
- Flags above are suggested defaults; adjust `OLLAMA_MAX_VRAM` to match your device.

### Status

Experimental. Feedback and fixes welcome.
