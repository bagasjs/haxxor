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

### Docs
```cpp:
bool InitHaxxor(const char* name, float width, float height)
void ShutHaxxor()
bool ShouldClose()
void PollEvents()
void SwapBuffers() // already included in EndDraw()

void BeginDraw()
void EndDraw()
void DrawRectangle(RECTANGLE r, COLOR c)
void DrawRectangleTex(RECTANGLE r, TEXTURE2D t)

IMAGE* LoadImage(const void* data, int width, int height)
IMAGE* LoadImageFromFile(const char* path, bool flip)
RECTANGLE GetImageShape(const IMAGE* img)
void DestroyImage(IMAGE* image)
TEXTURE2D LoadTextureFromImage(const IMAGE* image)
``` 