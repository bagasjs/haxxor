#include "hxgl.h"
#include <glad/glad.h>
#include <memory.h>

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

const GLchar* defaultVertSource = 
    "#version 430 core\n"
    "in vec2 position;\n"
    "in vec3 color;\n"
    "out vec3 Color;\n"
    "void main()\n"
    "{"
        "Color = color;\n"
        "gl_Position = vec4(position, 0.0, 1.0);\n"
    "}";

const GLchar* defaultFragSource =
    "#version 430 core\n"
    "in vec3 Color;\n"
    "out vec4 outColor;\n"
    "void main()\n"
    "{\n"
        "outColor = vec4(Color, 1.0);\n"
    "}\n";

/** Other functionality */

typedef struct HXGLCoreData {
    bool Initialized;
    uint32_t DefaultShader;
} HXGLCoreData;

static HXGLCoreData HXGL;

bool hxglInit()
{
    if(HXGL.Initialized) return false; // HXGL Already initialized
    memset(&HXGL, 0, sizeof(HXGLCoreData));
    HXGL.DefaultShader = hxglLoadShader(defaultVertSource, defaultFragSource);
    hxglEnableShader(HXGL.DefaultShader);
    HXGL.Initialized = true;
    return true;
}

void hxglTerminate()
{
    if(!HXGL.Initialized) return;
    hxglDropShader(HXGL.DefaultShader);
    HXGL.Initialized = false;
}

void hxglLoadExtension(void* loader)
{
    gladLoadGLLoader((GLADloadproc)loader);
}

void hxglClearColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
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

uint32_t hxglLoadShader(const char* vertSource, const char* fragSource)
{
    // Create and compile the vertex shader
    uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertSource, NULL);
    glCompileShader(vertexShader);

    // Create and compile the fragment shader
    uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragSource, NULL);
    glCompileShader(fragmentShader);

    // Link the vertex and fragment shader into a shader program
    uint32_t shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glUseProgram(shaderProgram);
    return shaderProgram;
}

void hxglDropShader(uint32_t shader)
{
    glDeleteProgram(shader);
}

void hxglEnableShader(uint32_t shader)
{
    glUseProgram(shader);
}

void hxglDisableShader()
{
    if(HXGL.Initialized)
        glUseProgram(HXGL.DefaultShader);
    else
        glUseProgram(0);
}