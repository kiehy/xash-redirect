# Metamod Plugin Template

A template for building Metamod plugins for GoldSrc and Xash3D FWGS dedicated servers, with optional [ReHLDS](https://github.com/dreamstalker/rehlds) API support.

Primarily targets **[Metamod-FWGS](https://github.com/FWGS/metamod-fwgs)**, but remains compatible with other Metamod forks including **[Metamod-R](https://github.com/theAsmodai/metamod-r)** and stock **Metamod-P**.

## Requirements

- **CMake** ≥ 3.12 (Presets require ≥ 3.21)
- **[Ninja](https://ninja-build.org/)** build system
- A C++11 capable compiler:
  - **Windows:** MSVC 2017+ (or clang-cl), MinGW GCC/Clang
  - **Linux:** GCC or Clang

## Quick Start

```sh
# Configure (pick a preset for your platform)
cmake --preset win-x64              # Windows MSVC x64
cmake --preset linux-x64-release    # Linux GCC x86_64

# Build
cmake --build --preset win-x64-release
cmake --build --preset linux-x64-release
```

**Windows note:** Run from the matching VS Native Tools Command Prompt (`x64` or `x86`) to select the target architecture.

## Presets

| Preset | Compiler | Arch |
|--------|----------|------|
| `win-x64` / `win-x86` | MSVC | x64 / x86 |
| `linux-x64-release` / `linux-x64-debug` | GCC | x86_64 |
| `linux-x86-release` / `linux-x86-debug` | GCC | x86 (`-m32`) |
| `linux-x64-clang-release` / `linux-x64-clang-debug` | Clang | x86_64 |
| `linux-x86-clang-release` / `linux-x86-clang-debug` | Clang | x86 (`-m32`) |

All presets use the **Ninja** generator. See `CMakePresets.json` for the full list.

## Customizing

1. **Rename the plugin**: edit `project()` in `CMakeLists.txt`, the `LIBRARY` line in `example_plugin.def`, and `Plugin_info` in `meta_api.cpp`.
2. **Fill in `Plugin_info`**: set name, version, author, URL, log tag, and load/unload times (`PT_ANYTIME`, `PT_STARTUP`, `PT_CHANGELEVEL`, etc.).
3. **Add hooks**: replace `NULL` entries in the function tables inside `engine_api.cpp` and `dllapi.cpp` with your own handler functions.
4. **ReHLDS hooks**: if using ReHLDS, use `g_RehldsHookchains` to install engine-level hook chains after `meta_init_rehlds_api()` succeeds.
