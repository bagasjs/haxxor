#ifndef __HXGL_H__
#define __HXGL_H__

#include <stdint.h>
#include <stdbool.h>

bool hxglInit();
void hxglLoadExtension(void* loader);
void hxglCheckErrors();
void hxglClear();
void hxglClearColor(float r, float g, float b, float a);

uint32_t hxglLoadVertexArray();
void hxglDropVertexArray(uint32_t vao);
void hxglEnableVertexArray(uint32_t vao);
void hxglDisableVertexArray();
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

uint32_t hxglLoadShader(const char* vertSource, const char* fragSource);
void hxglDropShader(uint32_t shader);
void hxglEnableShader(uint32_t shader);
void hxglDisableShader();


typedef enum HXGLAttrType {
    HXGL_BYTE = 0x1400,
    HXGL_UNSIGNED_BYTE = 0x1401,
    HXGL_SHORT = 0x1402,
    HXGL_UNSIGNED_SHORT = 0x1403,
    HXGL_INT = 0x1404,
    HXGL_UNSIGNED_INT = 0x1405,
    HXGL_FLOAT = 0x1406
} HXGLAttrType;

#endif 