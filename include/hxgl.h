#ifndef __HXGL_H__
#define __HXGL_H__

#include <stdint.h>
#include <stdbool.h>

void hxglLoadExtension(void* loader);
void hxglClear();

uint32_t hxglLoadVertexArray();
void hxglSetVertexAttribute(unsigned int index, int compCount, int type, bool normalized, int stride, const void *pointer);
void hxglDrawVertexArray(int offset, int count);
void hxglDrawVertexArrayElements(int offset, int count, const void* buffer);

uint32_t hxglLoadVertexBuffer(const void* data, int size, bool dynamic);
void hxglDropVertexBuffer(uint32_t vbo);
void hxglUpdateVertexBuffer(uint32_t vbo, const void* data, int dataSize, int offset);
void hxglEnableVertexBuffer(uint32_t vbo);
void hxglDisableVertexBuffer();

uint32_t hxglLoadIndexBuffer(const void* data, int size, bool dynamic);
void hxglDropIndexBuffer(uint32_t ibo);
void hxglUpdateIndexBuffer(uint32_t ibo, const void* data, int dataSize, int offset);
void hxglEnableIndexBuffer(uint32_t ibo);
void hxglDisableIndexBuffer();

typedef enum HXGLAttrType {
    GL_BYTE = 0x1400,
    GL_UNSIGNED_BYTE = 0x1401,
    GL_SHORT = 0x1402,
    GL_UNSIGNED_SHORT = 0x1403,
    GL_INT = 0x1404,
    GL_UNSIGNED_INT = 0x1405,
    GL_FLOAT = 0x1406
} HXGLAttrType;

#endif 