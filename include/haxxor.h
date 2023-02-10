#ifndef __HAXXOR_H__
#define __HAXXOR_H__

#include <stdbool.h>
#include <stdint.h>

#define MAXIMUM_VERTICES 5000
#define MAXIMUM_ELEMENTS MAXIMUM_VERTICES * 4
#define MAXIMUM_TEXTURE_SLOT 10 // currently not able to be modified

typedef struct RECTANGLE {
    float x, y, w, h;
} RECTANGLE;

typedef struct COLOR {
    uint8_t r, g, b, a;
} COLOR;

typedef unsigned int TEXTURE2D;
typedef struct IMAGE IMAGE;

bool InitHaxxor(const char* name, float width, float height);
bool ShouldClose();
void PollEvents();
void SwapBuffers();
void ShutHaxxor();

IMAGE* LoadImage(const void* data, int width, int height);
IMAGE* LoadImageFromFile(const char* path, bool flip);
RECTANGLE GetImageShape(const IMAGE* img);
void DestroyImage(IMAGE* image);
TEXTURE2D LoadTextureFromImage(const IMAGE* image);

void BeginDraw();
void EndDraw();
void DrawRectangle(RECTANGLE r, COLOR c);
void DrawRectangleTex(RECTANGLE r, TEXTURE2D t);

#endif