#ifndef __HXGL_H__
#define __HXGL_H__

#include <stdint.h>
#include <stdbool.h>

#define HXGL_TEX_SLOT_CAPACITY 10

bool hxglInit();
void hxglUseExtension(void* loader);
void hxglCheckErrors();
void hxglClear();
void hxglClearColor(float r, float g, float b, float a);

uint32_t hxglLoadVertexArray();
void hxglDropVertexArray(uint32_t vao);
void hxglEnableVertexArray(uint32_t vao);
void hxglDisableVertexArray();
void hxglSetVertexAttribute(unsigned int index, int compCount, int attrKind, bool normalized, int vertexSize, const void *vertexAttrOffset);
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
int hxglGetUniformLocation(uint32_t shader, const char* name);
void hxglSetUniform(int location, const void* value, int uniformKind, int count);
void hxglSetUniformMat4(int location, const float* value);


uint32_t hxglLoadTexture(const void* data, int width, int height, int filter);
void hxglEnableTexture(uint32_t texture, int slot);
void hxglDisableTexture();


typedef enum HXGLAttrKind {
    HXGL_BYTE = 0x1400,
    HXGL_UNSIGNED_BYTE = 0x1401,
    HXGL_SHORT = 0x1402,
    HXGL_UNSIGNED_SHORT = 0x1403,
    HXGL_INT = 0x1404,
    HXGL_UNSIGNED_INT = 0x1405,
    HXGL_FLOAT = 0x1406
} HXGLAttrKind;

typedef enum HXGLShaderUniformKind {
    HXGL_SHADER_UNIFORM_FLOAT = 0,
    HXGL_SHADER_UNIFORM_VEC2,
    HXGL_SHADER_UNIFORM_VEC3,
    HXGL_SHADER_UNIFORM_VEC4,
    HXGL_SHADER_UNIFORM_INT,
    HXGL_SHADER_UNIFORM_IVEC2,
    HXGL_SHADER_UNIFORM_IVEC3,
    HXGL_SHADER_UNIFORM_IVEC4,
    HXGL_SHADER_UNIFORM_SAMPLER2D
} HXGLShaderUniformKind;

typedef enum HXGLTextureFilter {
    HXGL_LINEAR = 0x2600,
    HXGL_NEAREST = 0x2601,
    HXGL_NEAREST_MIPMAP_NEAREST = 0x2700,
    HXGL_LINEAR_MIPMAP_NEAREST = 0x2701,
    HXGL_NEAREST_MIPMAP_LINEAR = 0x2702,
    HXGL_LINEAR_MIPMAP_LINEAR = 0x2703,
} HXGLTextureFilter;

#ifdef HXGL_MAKE_IMPLEMENTATION
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
                    case GL_NO_ERROR: check = 0; break;
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
        "layout(location = 0) in vec3 a_Position;\n"
        "layout(location = 1) in vec4 a_Color;\n"
        "layout(location = 2) in vec2 a_TexCoords;\n"
        "layout(location = 3) in float a_TexId;\n"
        "out vec4 v_Color;\n"
        "void main()\n"
        "{"
            "v_Color = a_Color;\n"
            "gl_Position = vec4(a_Position, 1.0);\n"
            // "gl_Position = a_Color;\n"
        "}";

    const GLchar* defaultFragSource =
        "#version 430 core\n"
        "layout(location = 0) out vec4 outColor;\n"
        "in vec4 v_Color;\n"
        "void main()\n"
        "{\n"
            "outColor = v_Color;\n"
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
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        HXGL.Initialized = true;
        return true;
    }

    void hxglTerminate()
    {
        if(!HXGL.Initialized) return;
        hxglDropShader(HXGL.DefaultShader);
        HXGL.Initialized = false;
    }

    void hxglUseExtension(void* loader)
    {
        gladLoadGLLoader((GLADloadproc)loader);
    }

    void hxglClearColor(float r, float g, float b, float a)
    {
        glClearColor(r, g, b, a);
    }

    void hxglClear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glUseProgram(shaderProgram);
        hxglCheckErrors();
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

    int hxglGetUniformLocation(uint32_t shader, const char* name)
    {
        int loc = -1;
        loc = glGetUniformLocation(shader, name);
        if(loc == -1) LOG_WARN("Failed to find uniform with name %s", name);
        return loc;
    }

    void hxglSetUniform(int location, const void* value, int uniformKind, int count)
    {
        switch(uniformKind)
        {
            case HXGL_SHADER_UNIFORM_FLOAT: glUniform1fv(location, count, (float*)value); break;
            case HXGL_SHADER_UNIFORM_VEC2: glUniform2fv(location, count, (float*)value); break;
            case HXGL_SHADER_UNIFORM_VEC3: glUniform3fv(location, count, (float*)value); break;
            case HXGL_SHADER_UNIFORM_VEC4: glUniform4fv(location, count, (float*)value); break;
            case HXGL_SHADER_UNIFORM_INT: glUniform1iv(location, count, (int*)value); break;
            case HXGL_SHADER_UNIFORM_IVEC2: glUniform2iv(location, count, (int*)value); break;
            case HXGL_SHADER_UNIFORM_IVEC3: glUniform3iv(location, count, (int*)value); break;
            case HXGL_SHADER_UNIFORM_IVEC4: glUniform4iv(location, count, (int*)value); break;
            case HXGL_SHADER_UNIFORM_SAMPLER2D: glUniform1iv(location, count, (int*)value); break;
            default: LOG_WARN("Invalid shader uniform kind: %d", uniformKind);
        }
    }

    void hxglSetUniformMat4(int location, const float* value)
    {
        glUniformMatrix4fv(location, 1, false, value);
    }


    uint32_t hxglLoadTexture(const void* data, int width, int height, int filter)
    {
        uint32_t tex = 0;
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        return tex;
    }

    void hxglEnableTexture(uint32_t texture, int slot)
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, texture);
    }

    void hxglDisableTexture()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
#endif // HXGL_MAKE_IMPLEMENTATION

#endif // __HXGL_H__