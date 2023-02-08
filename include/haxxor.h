#ifndef __HAXXOR_H__
#define __HAXXOR_H__

#include <stdbool.h>
#include <stdint.h>

#define MAXIMUM_VERTICES 5000
#define MAXIMUM_ELEMENTS MAXIMUM_VERTICES * 4

typedef struct RECTANGLE {
    float x, y, w, h;
} RECTANGLE;

typedef struct COLOUR {
    uint8_t r, g, b, a;
} COLOUR;

bool InitHaxxor(const char* name, float width, float height);
bool ShouldClose();
void Update();
void PollEvents();
void ShutHaxxor();

#endif