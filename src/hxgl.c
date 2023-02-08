#include "hxgl.h"
#include <glad/glad.h>

/** Other functionality */
void hxglLoadExtension(void* loader)
{
    gladLoadGLLoader((GLADloadproc)loader);
}

void hxglClear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

/** Vertex Array */
uint32_t hxglLoadVertexArray()
{
    uint32_t vao = 0;
    glGenVertexArrays(1, &vao);
    return vao;
}

void hxglSetVertexAttribute(unsigned int index, int compCount, int type, bool normalized, int stride, const void *pointer)
{
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, compCount, type, normalized, stride, pointer);
}

void hxglDrawVertexArray(int offset, int count)
{
    glDrawArrays(GL_TRIANGLES, offset, count);
}

void hxglDrawVertexArrayElements(int offset, int count, const void* buffer)
{
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (const uint32_t*)buffer + offset);
}


/** Vertex Buffer */
uint32_t hxglLoadVertexBuffer(const void* data, int size, bool dynamic)
{
    uint32_t vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, data, dynamic? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    return vbo;
}

void hxglDropVertexBuffer(uint32_t vbo)
{
    glDeleteBuffers(1, &vbo);
}

void hxglUpdateVertexBuffer(uint32_t vbo, const void* data, int dataSize, int offset)
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, offset, dataSize, data);
}

void hxglEnableVertexBuffer(uint32_t vbo)
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

void hxglDisableVertexBuffer()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


/** Index Buffer */
uint32_t hxglLoadIndexBuffer(const void* data, int size, bool dynamic)
{
    uint32_t vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, dynamic? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    return vbo;
}

void hxglDropIndexBuffer(uint32_t vbo)
{
    glDeleteBuffers(1, &vbo);
}

void hxglUpdateIndexBuffer(uint32_t ibo, const void* data, int dataSize, int offset)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, dataSize, data);
}

void hxglEnableIndexBuffer(uint32_t vbo)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
}

void hxglDisableIndexBuffer()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}