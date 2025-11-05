# Curloz Engine

A modern, cross-platform 3D game engine built with OpenGL, featuring advanced rendering techniques and flexible scripting capabilities.

## Features

### Rendering
- **OpenGL 3.3 Core Profile** - Modern graphics pipeline
- **Blinn-Phong Lighting Model** - Realistic lighting and shading
- **MSAA (Multisample Anti-Aliasing)** - Smooth edge rendering with 4x multisampling
- **Framebuffer System** - Offscreen rendering with post-processing support
- **Skybox/Cubemap Support** - Environment mapping
- **Model Loading** - Assimp integration for importing 3D models

### Architecture
- **Entity Component System** - Flexible game object management
- **Camera System** - Free-flight camera with smooth controls
- **Lighting System** - Dynamic light management
- **Shader Manager** - Hot-reloadable GLSL shaders

### Developer Tools
- **ImGui Integration** - In-engine debug UI
- **Edit/Game Mode Toggle** - Runtime editor capabilities
- **JSON/TOML Scripting** - Configuration and scene serialization
- **Cross-Platform** - Runs on Windows, Linux, and macOS

## Dependencies

- **GLFW 3.x** - Window and input management
- **GLAD 1.** - OpenGL loader
- **GLM** - Mathematics library
- **Assimp** - 3D model loading
- **ImGui** - Immediate mode GUI

## Building

### Prerequisites
```bash
# Install Make
# Install CMake (3.10+)
# Install a C++17 compatible compiler (GCC, Clang, MSVC)
```

### Clone and Build
```bash
git clone https://github.com/culoz123/curloz-engine.git
cd curloz-engine
```

### Configure and build
```bash
# For Current-time-being in development phase, it is recommended to build the engine in just DEBUG mode
 
# 1. Just use make
# Supported TOOLCHAIN's are clang, gcc and mingw.
# MOLD's support is also added, just pass USE_MOLD=1, by default it is set to 0.
make TOOLCHAIN=gcc USE_MOLD=1 DEBUG=1 -j$(nproc)

# 2. USE Cmake
cd build && cmake .. && make -j$(nproc)
```

# Run
```bash
#If built engine using makefile, just do make run from root directory
make run
#Else, do ./build/prog
./build/prog
```

## Controls

### Game Mode
- **W/A/S/D** - Move camera
- **Mouse** - Look around
- **Scroll** - Adjust FOV/Zoom
- **ESC** - Exit

### Debug Mode
- **TAB** - Switch to edit mode
- **CTRL+G** - Return to game mode

## Project Structure
```
curloz-engine/
├── assets/
│   ├── shaders/       # GLSL vertex/fragment shaders
│   ├── models/        # 3D model files
│   └── textures/      # Texture assets
├── include/
│   ├── entity/        # Entity system
│   ├── global/        # Global managers
│   ├── lighting/      # Lighting system
│   ├── render/        # Rendering pipeline
│   └── shader/        # Shader management
├── src/               # Implementation files
└── lib/               # External libraries (not tracked)
```

## Configuration

Engine settings can be configured through JSON/TOML files for:
- Window resolution and display settings
- Graphics quality options
- Input mappings
- Scene definitions

## Roadmap

- [ ] Deferred rendering pipeline
- [ ] PBR (Physically Based Rendering)
- [ ] Shadow mapping
- [ ] Post-processing effects (bloom, HDR, SSAO)
- [ ] Audio system integration
- [ ] Physics engine integration
- [ ] Scene serialization/deserialization

## License

This project is licensed under the **GNU Lesser General Public License v3.0** (LGPL v3).

[![License: LGPL v3](https://img.shields.io/badge/License-LGPL%20v3-blue.svg)](https://www.gnu.org/licenses/lgpl-3.0)

## Acknowledgments

- [LearnOpenGL](https://learnopengl.com/) - Excellent OpenGL tutorials
- Joey de Vries - OpenGL learning resources
- The open-source community for the amazing libraries

## Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for bugs and feature requests.

---

**Note:** This is an educational project built while learning OpenGL and game engine architecture.
