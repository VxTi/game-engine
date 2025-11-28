# Open World Game Engine

---

This project is a learning exercise focused on building a performant open world game engine in C++20, with an emphasis on rendering optimizations and modern graphics techniques. It uses OpenGL for rendering and GLFW for window/context management.

## Features

- Written in modern C++ (C++20)
- Uses OpenGL (3.3+) for graphics rendering
- GLFW for cross-platform window and input handling
- GLM for mathematics and vector operations
- stb_image for image loading
- Modular codebase for extensibility

## Automatic Dependency Management

**You do NOT need to manually install any dependencies.**  
All required libraries (GLFW, GLM, stb_image) are automatically downloaded and configured via CMake's dependency manager.  
Just follow the setup steps below.

## Getting Started

### 1. Clone the Repository

```sh
git clone https://github.com/yourusername/game-engine.git
cd game-engine
```

### 2. Build the Project

Make sure you have CMake (>= 3.16) and a C++20-compatible compiler installed.

#### On macOS / Linux / Windows

```sh
cmake -S . -B build
cmake --build build
```

This will automatically fetch and build all dependencies.

### 3. Run the Engine

After building, the executable will be located in the `build` directory:

```sh
./build/GameEngine
```

## Directory Structure

- `src/` — Main source code
- `include/` — Public headers
- `shaders/` — GLSL shader programs
- `documentation/` — Design notes and documentation
- `build/` — CMake build output (created after building)

## Contributing

Contributions, suggestions, and bug reports are welcome!  
Please open an issue or submit a pull request.

## License

This project is released under the MIT License.  
See [LICENSE](LICENSE) for details.