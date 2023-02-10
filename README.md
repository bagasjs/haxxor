# Haxxor 
A simple game making library for C/C++ hugely inspired by [Raylib](https://github.com/raysan5/raylib/) and [SDL](https://github.com/libsdl-org/SDL). Currently only support 2D Rendering. 

### Features
- All dependency is included is the project
- Math library support 2, 3, and 4 components vector and 4x4 matrix
- An abstraction of OpenGL hugely inspired by [rlgl.h](https://github.com/raysan5/raylib/blob/master/src/rlgl.h)
- A python based build engine. it will not always work as it should. Thereby you might need to modify the **build.py** file.

### Dependencies
- [GLFW](https://github.com/glfw/glfw)
- [glad](https://github.com/Dav1dde/glad)
- [stb_image.h](https://github.com/nothings/stb)
- [miniaudio](https://github.com/mackron/miniaudio)

### Supported platform
Haxxor is currently only available for windows and every linux system that based on X11.

### Future Update (maybe)
- Add audio library
- Camera
- 3D Support

### How to use
You can look at the **example** directory in this repository. Beside that you should also read the **include/haxxor.h** file to understand the API