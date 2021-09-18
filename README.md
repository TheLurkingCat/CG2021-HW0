# CG2021 Homework 0

## Dependencies

- [glfw3](https://github.com/glfw/glfw)
- [glad2](https://github.com/Dav1dde/glad)

### If you are building on Linux, you need one of these dependencies, usually `xorg-dev`
- `xorg-dev` (For X11)
- `libwayland-dev wayland-protocols extra-cmake-modules libxkbcommon-dev` (For Wayland)
- `libosmesa6-dev` (For OSMesa)

## Instruction

### CMake

```bash=
cmake -S . -B build
cmake --build build --config Release --parallel 8
cd bin
./HW0
```

### Visual Studio 2019

- Open `CG2021.sln`
- Set `release` mode then build
- Executable will be in ./bin
