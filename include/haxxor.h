#ifndef __HAXXOR_H__
#define __HAXXOR_H__

#include <stdbool.h>
#include <stdint.h>

#define MAXIMUM_VERTICES 5000
#define MAXIMUM_ELEMENTS MAXIMUM_VERTICES * 4

typedef struct RECTANGLE {
    float x, y, w, h;
} RECTANGLE;

typedef struct COLOR {
    uint8_t r, g, b, a;
} COLOR;

bool InitHaxxor(const char* name, float width, float height);
bool ShouldClose();
void PollEvents();
void SwapBuffers();
void ShutHaxxor();

void BeginDraw();
void EndDraw();
void DrawRectangle(RECTANGLE r, COLOR c);

#endif