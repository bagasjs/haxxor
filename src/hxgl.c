#include "hxgl.h"
#include <glad/glad.h>

/** Other functionality */
void hxglLoadExtension(void* loader)
{
    gladLoadGLLoader((GLADloadproc)loader);
}

#ifndef HXGL_BUILD_RELEASE
    #include <stdio.h>
    #define LOG_INFO(FMT, ...) printf("[INFO]: " FMT "\n", __VA_ARGS__)
    #define LOG_WARN(FMT, ...) printf("[WARN]: " FMT "\n", __VA_ARGS__)
    #define LOG_ERROR(FMT, ...) printf("[ERROR]: " FMT "\n", __VA_ARGS__)

    void hxglCheckErrors()
    {
        int check = 1;
        while (check)
        {
            const GLenum err = glGetError();
            switch (err)
            {
                case GL_NO_ERROR: LOG_INFO("%s", "No OpenGL error detected"); check = 0; break;
                case 0x0500: LOG_WARN("%s", "OpenGL Error detected: GL_INVALID_ENUM"); break;
                case 0x0501: LOG_WARN("%s", "OpenGL Error detected: GL_INVALID_VALUE"); break;
                case 0x0502: LOG_WARN("%s", "OpenGL Error detected: GL_INVALID_OPERATION"); break;
                case 0x0503: LOG_WARN("%s", "OpenGL Error detected: GL_STACK_OVERFLOW"); break;
                case 0x0504: LOG_WARN("%s", "OpenGL Error detected: GL_STACK_UNDERFLOW"); break;
                case 0x0505: LOG_WARN("%s", "OpenGL Error detected: GL_OUT_OF_MEMORY"); break;
                case 0x0506: LOG_WARN("%s", "OpenGL Error detected: GL_INVALID_FRAMEBUFFER_OPERATION"); break;
                default: LOG_WARN("%s:%i", "OpenGL Error detected: Unknown error code:", err); break;
            }
        }
    }
#else
    void hxglCheckErrors() { }
    #define LOG_INFO(FMT, ...)
    #define LOG_WARN(FMT, ...)
    #define LOG_ERROR(FMT, ...)
#endif

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

void hxglDropVertexArray(uint32_t vao)
{
    glDeleteVertexArrays(1, &vao);
}

void hxglEnableVertexArray(uint32_t vao)
{
    glBindVertexArray(vao);
}

void hxglDisableVertexArray()
{
    glBindVertexArray(0);
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
    glBufferData(GL_ARRAY_BUFFER, size, data, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
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