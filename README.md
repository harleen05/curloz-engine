## Project Overview

### Description
curloz-engine is a lightweight, modern Vulkan-based graphics engine written in C++, designed for simplicity, educational clarity, and rapid prototyping. It strips away heavy abstractions to give developers direct, readable access to Vulkan's core pipeline and window management systems.

### Tech Stack
- **Language:** C++17/20
- **Graphics API:** Vulkan 1.3+
- **Build System:** xmake
- **Shaders:** GLSL → SPIR-V (via `glslc`)
- **Windowing:** Custom abstraction over native APIs
- **Other Tools:** Vulkan Validation Layers, glslc, xmake

### Current Features
curloz-engine currently supports:
- Core window initialization & event loop
- Vulkan instance, device, and swapchain creation
- Graphics pipeline setup (programmable stages + fixed-function configuration)
- SPIR-V shader loading & pipeline compilation
- Basic render loop & frame presentation
- Structured asset management (`assets/shaders/`, `assets/models/`, `assets/config/`)
- Cross-platform project structure (Linux-optimized initially)

### Target Users
This project is built for graphics programmers, Vulkan learners, and C++ developers looking for a clean, minimal Vulkan engine template without heavy framework abstraction or hidden magic.

---

## Architecture / Key Modules

### Module Overview
| Module/Component | Location | Purpose |
|------------------|----------|---------|
| `base` | `src/base.cpp`, `include/base.hpp` | Core engine initialization, main loop, global state management |
| `window` | `src/window/`, `include/window/` | Window creation, surface setup, render context, event polling |
| `renderer` | `src/renderer/`, `include/renderer/` | Vulkan initialization, pipeline creation, resource cleanup, render types |
| `assets` | `assets/` | Shaders (`.glsl`/`.spv`), 3D models, configuration files |
| `xmake.lua` | `xmake.lua` | Cross-platform build configuration, compilation rules, runtime directories |

### Project Structure
```bash
Curloz Engine
├── assets
│   ├── config                      // Config files
│   ├── models                      // All 3d models
│   └── shaders                     // Shaders
│       ├── triangle.frag
│       ├── triangle_frag.spirv
│       ├── triangle.vert
│       └── triangle_vert.spirv
├── build                           // Build directory
├── COPYING                         // License
├── include                         // Include directory
│   ├── base.hpp
│   ├── clz_types.hpp
│   ├── renderer
│   │   ├── cleaners.hpp
│   │   ├── initializers.hpp
│   │   ├── renderer.hpp
│   │   └── render_types.hpp
│   └── window
│       ├── render.hpp
│       ├── window.hpp
│       ├── window_types.hpp
│       └── window_variables.hpp
├── README.md
├── src                             // Source files
│   ├── base.cpp
│   ├── main.cpp
│   ├── renderer
│   │   ├── cleaners.cpp
│   │   ├── initializers.cpp
│   │   └── renderer.cpp
│   └── window
│       ├── render.cpp
│       └── window.cpp
└── xmake.lua                       // Build system
```

## New Feature Ideas

### Feature 1: Windows xmake Compatibility & Cross-Platform Validation
**Problem it solves:** The engine is currently optimized and tested on Linux. Windows Vulkan development uses different toolchains (MSVC/MinGW), Vulkan SDK discovery methods, and path separators. Contributors on Windows often face build or runtime errors.
**Difficulty Level:** Intermediate
**Estimated Effort:** 6-10 hours
**Modules Affected:**
- `xmake.lua` - Cross-platform toolchain & package configuration
- `src/renderer/initializers.cpp` - Path normalization for asset loading
- `README.md` - Windows setup instructions
- `assets/` - Runtime directory handling

### Feature 2: Automatic Shader Compilation via xmake
**Problem it solves:** Contributors currently need to manually run `glslc` to compile `.vert` or `frag` to `.spv` before testing changes.
**Difficulty Level:** Beginner
**Estimated Effort:** 3-5 hours
**Modules Affected:**
- `xmake.lua` - Add `on_build`/`after_build` rules for shader compilation
- `assets/shaders/` - Standardize naming conventions

##  Contributor Notes

### Getting Started
#### Prerequisites
Make sure you have the following installed:
- [Git](https://git-scm.com/)
- [xmake](https://xmake.io/) (v2.8.0+)
- [Vulkan SDK 1.3+](https://vulkan.lunarg.com/)
- C++ Compiler: `g++`/`clang++` (Linux/macOS) or `MSVC`/`MinGW` (Windows)
- `glslc` (included with Vulkan SDK)

#### Workflow
```bash
# Need to run only the first time
xmake f -c
# Compile project and run
xmake && xmake run
```

In case of any issues, contact me at discord ID: itz_curloz_36825
or email me at: curloz@tutamail.com.

Happy contributing
