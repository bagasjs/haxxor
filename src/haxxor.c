#include "haxxor.h"
#include "hxgl.h"
#include "hxmath.h"
#include <GLFW/glfw3.h>

/** Utilities */
VEC4 ColorToVec4(COLOR col)
{
    return Vec4Create(col.r / 255.0f, col.g / 255.0f, col.b / 255.0f, col.a / 255.0f);
}


typedef struct Vertex {
    VEC3 Pos;
    VEC4 Color;
    VEC2 TexCoords;
    float TexID;
} Vertex;

/** Haxxor */
typedef struct Application {
    bool Initialized;
    struct {
        GLFWwindow* Handle;
    } Surface;
    struct {
        uint32_t VAO, VBO, IBO;
        Vertex Vertices[MAXIMUM_VERTICES];
        uint32_t Elements[MAXIMUM_ELEMENTS];
        uint32_t ElementsCount, VerticesCount;
    } Renderer;
} Application;

static Application APP = {0};

bool InitHaxxor(const char* name, float width, float height)
{
    if(APP.Initialized) return false; // Haxxor has been initialized
    // Window Initialization
    if(!glfwInit()) return false; // Failed to initialize glfw
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    APP.Surface.Handle = glfwCreateWindow((int)width, (int)height, name, NULL, NULL);
    if(APP.Surface.Handle == NULL) return false; // Failed to create window
    glfwMakeContextCurrent(APP.Surface.Handle);

    // Renderer Initialization
    hxglLoadExtension(glfwGetProcAddress);
    if(!hxglInit()) return false;
    memset(APP.Renderer.Vertices, 0, sizeof(APP.Renderer.Vertices));
    memset(APP.Renderer.Elements, 0, sizeof(APP.Renderer.Elements));
    APP.Renderer.VAO = hxglLoadVertexArray();
    APP.Renderer.VBO = hxglLoadVertexBuffer(NULL, sizeof(APP.Renderer.Vertices), true);
    APP.Renderer.IBO = hxglLoadIndexBuffer(NULL, sizeof(APP.Renderer.Elements), true);
    hxglEnableVertexArray(APP.Renderer.VAO);
    hxglEnableVertexBuffer(APP.Renderer.VBO);
    hxglSetVertexAttribute(0, 3, HXGL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, Pos));
    hxglSetVertexAttribute(1, 4, HXGL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, Color));
    hxglSetVertexAttribute(2, 2, HXGL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    hxglSetVertexAttribute(3, 1, HXGL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, TexID));

    APP.Initialized = true;
    return true;
}

void ShutHaxxor()
{
    if(!APP.Initialized) return;
    glfwDestroyWindow(APP.Surface.Handle);
    glfwTerminate();   
    APP.Initialized = false;
}

bool ShouldClose()
{
    return glfwWindowShouldClose(APP.Surface.Handle);
}

void PollEvents()
{
    glfwPollEvents();
}

void SwapBuffers()
{
    glfwSwapBuffers(APP.Surface.Handle);
}

void BeginDraw()
{
    // Clean up
    hxglClear();
    memset(APP.Renderer.Vertices, 0, sizeof(APP.Renderer.Vertices));
    memset(APP.Renderer.Elements, 0, sizeof(APP.Renderer.Elements));
    hxglDisableVertexArray();
    hxglDisableVertexBuffer();
    hxglDisableIndexBuffer();
}

void EndDraw()
{
    // Draw to screen
    hxglEnableVertexArray(APP.Renderer.VAO);
    hxglEnableVertexBuffer(APP.Renderer.VBO);
    hxglEnableIndexBuffer(APP.Renderer.IBO);

    hxglUpdateVertexBuffer(APP.Renderer.VBO, APP.Renderer.Vertices, APP.Renderer.VerticesCount * sizeof(Vertex), 0);
    hxglUpdateIndexBuffer(APP.Renderer.IBO, APP.Renderer.Elements, APP.Renderer.ElementsCount * sizeof(uint32_t), 0);

    hxglDrawVertexArrayElements(0, APP.Renderer.ElementsCount, 0);
    SwapBuffers();
}


void DrawRectangle(RECTANGLE r, COLOR c)
{
    APP.Renderer.Elements[APP.Renderer.ElementsCount + 0] = APP.Renderer.VerticesCount + 0;
    APP.Renderer.Elements[APP.Renderer.ElementsCount + 1] = APP.Renderer.VerticesCount + 1;
    APP.Renderer.Elements[APP.Renderer.ElementsCount + 2] = APP.Renderer.VerticesCount + 2;
    APP.Renderer.Elements[APP.Renderer.ElementsCount + 3] = APP.Renderer.VerticesCount + 2;
    APP.Renderer.Elements[APP.Renderer.ElementsCount + 4] = APP.Renderer.VerticesCount + 3;
    APP.Renderer.Elements[APP.Renderer.ElementsCount + 5] = APP.Renderer.VerticesCount + 0;
    APP.Renderer.ElementsCount += 6;

    VEC4 rcol = ColorToVec4(c);
    APP.Renderer.Vertices[APP.Renderer.VerticesCount + 0].Pos.x = r.x;
    APP.Renderer.Vertices[APP.Renderer.VerticesCount + 0].Pos.y = r.y;
    APP.Renderer.Vertices[APP.Renderer.VerticesCount + 0].Color = rcol;

    APP.Renderer.Vertices[APP.Renderer.VerticesCount + 1].Pos.x = r.x + r.w;
    APP.Renderer.Vertices[APP.Renderer.VerticesCount + 1].Pos.y = r.y;
    APP.Renderer.Vertices[APP.Renderer.VerticesCount + 1].Color = rcol;

    APP.Renderer.Vertices[APP.Renderer.VerticesCount + 2].Pos.x = r.x + r.w;
    APP.Renderer.Vertices[APP.Renderer.VerticesCount + 2].Pos.y = r.y + r.h;
    APP.Renderer.Vertices[APP.Renderer.VerticesCount + 2].Color = rcol;

    APP.Renderer.Vertices[APP.Renderer.VerticesCount + 3].Pos.x = r.x;
    APP.Renderer.Vertices[APP.Renderer.VerticesCount + 3].Pos.y = r.y + r.h;
    APP.Renderer.Vertices[APP.Renderer.VerticesCount + 3].Color = rcol;

    APP.Renderer.VerticesCount += 4;
}