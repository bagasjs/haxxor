#define HXGL_MAKE_IMPLEMENTATION
#include "hxgl.h"
#include "haxxor.h"
#include "hxmath.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
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

struct IMAGE {
    bool LoadedFromFile;
    void* Data;
    int Width, Height;
};

/** Haxxor */
const GLchar* vertSource = 
    "#version 430 core\n"
    "layout(location = 0) in vec3 a_Position;\n"
    "layout(location = 1) in vec4 a_Color;\n"
    "layout(location = 2) in vec2 a_TexCoords;\n"
    "layout(location = 3) in float a_TexId;\n"
    "uniform mat4 u_WorldMatrix;\n"
    "out vec4 v_Color;\n"
    "out vec2 v_TexCoords;\n"
    "out float v_TexId;\n"
    "void main()\n"
    "{"
        "v_Color = a_Color;\n"
        "v_TexCoords = a_TexCoords;\n"
        "v_TexId = a_TexId;\n"
        "gl_Position = u_WorldMatrix * vec4(a_Position, 1.0);\n"
    "}";

const GLchar* fragSource =
    "#version 430 core\n"
    "layout(location = 0) out vec4 outColor;\n"
    "in vec4 v_Color;\n"
    "in vec2 v_TexCoords;\n"
    "in float v_TexId;\n"
    "uniform sampler2D u_Textures[10];\n" // make a more dynamic texture system
    "void main()\n"
    "{\n"
        "if(v_TexId >= 0) {\n"
        "outColor = texture(u_Textures[int(v_TexId)], v_TexCoords);\n"
        "} else {\n"
        "outColor = v_Color;\n"
        "}\n"
    "}\n";

typedef struct Application {
    bool Initialized;
    struct {
        GLFWwindow* Handle;
    } Surface;
    struct {
        uint32_t VAO, VBO, IBO, Shader;
        int NextAvailSlot;
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
    hxglUseExtension(glfwGetProcAddress);
    if(!hxglInit()) return false;
    APP.Renderer.Shader = hxglLoadShader(vertSource, fragSource);
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

    uint32_t wmloc = hxglGetUniformLocation(APP.Renderer.Shader, "u_WorldMatrix");
    MAT4 proj = Mat4Orthographic(0.0f, width, height, 0.0f, -1.0f, 1.0f);
    hxglSetUniformMat4(wmloc, proj.elements);

    APP.Renderer.NextAvailSlot = 0;

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
    APP.Renderer.VerticesCount = 0;
    APP.Renderer.ElementsCount = 0;
    APP.Renderer.NextAvailSlot = 0;
    // hxglDisableVertexArray();
    // hxglDisableVertexBuffer();
    // hxglDisableIndexBuffer();
    // hxglDisableShader();
}

void EndDraw()
{
    // Draw to screen
    hxglEnableShader(APP.Renderer.Shader);
    hxglEnableVertexArray(APP.Renderer.VAO);
    hxglEnableVertexBuffer(APP.Renderer.VBO);
    hxglEnableIndexBuffer(APP.Renderer.IBO);

    hxglUpdateVertexBuffer(APP.Renderer.VBO, APP.Renderer.Vertices, APP.Renderer.VerticesCount * sizeof(Vertex), 0);
    hxglUpdateIndexBuffer(APP.Renderer.IBO, APP.Renderer.Elements, APP.Renderer.ElementsCount * sizeof(uint32_t), 0);

    int utexloc = hxglGetUniformLocation(APP.Renderer.Shader, "u_Textures");
    int* samplers = malloc(sizeof(int) * 10);
    for(int i = 0; i < 10; i++)
    {
        if(i < APP.Renderer.NextAvailSlot) samplers[i] = i;
        else samplers[i] = -1;
    }
    hxglSetUniform(utexloc, samplers, HXGL_SHADER_UNIFORM_INT, APP.Renderer.NextAvailSlot);

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
    APP.Renderer.Vertices[APP.Renderer.VerticesCount + 0].TexID = -1;

    APP.Renderer.Vertices[APP.Renderer.VerticesCount + 1].Pos.x = r.x + r.w;
    APP.Renderer.Vertices[APP.Renderer.VerticesCount + 1].Pos.y = r.y;
    APP.Renderer.Vertices[APP.Renderer.VerticesCount + 1].Color = rcol;
    APP.Renderer.Vertices[APP.Renderer.VerticesCount + 1].TexID = -1;

    APP.Renderer.Vertices[APP.Renderer.VerticesCount + 2].Pos.x = r.x + r.w;
    APP.Renderer.Vertices[APP.Renderer.VerticesCount + 2].Pos.y = r.y + r.h;
    APP.Renderer.Vertices[APP.Renderer.VerticesCount + 2].Color = rcol;
    APP.Renderer.Vertices[APP.Renderer.VerticesCount + 2].TexID = -1;

    APP.Renderer.Vertices[APP.Renderer.VerticesCount + 3].Pos.x = r.x;
    APP.Renderer.Vertices[APP.Renderer.VerticesCount + 3].Pos.y = r.y + r.h;
    APP.Renderer.Vertices[APP.Renderer.VerticesCount + 3].Color = rcol;
    APP.Renderer.Vertices[APP.Renderer.VerticesCount + 3].TexID = -1;

    APP.Renderer.VerticesCount += 4;
}

void DrawRectangleTex(RECTANGLE r, TEXTURE2D t)
{
    APP.Renderer.Elements[APP.Renderer.ElementsCount + 0] = APP.Renderer.VerticesCount + 0;
    APP.Renderer.Elements[APP.Renderer.ElementsCount + 1] = APP.Renderer.VerticesCount + 1;
    APP.Renderer.Elements[APP.Renderer.ElementsCount + 2] = APP.Renderer.VerticesCount + 2;
    APP.Renderer.Elements[APP.Renderer.ElementsCount + 3] = APP.Renderer.VerticesCount + 2;
    APP.Renderer.Elements[APP.Renderer.ElementsCount + 4] = APP.Renderer.VerticesCount + 3;
    APP.Renderer.Elements[APP.Renderer.ElementsCount + 5] = APP.Renderer.VerticesCount + 0;
    APP.Renderer.ElementsCount += 6;

    hxglEnableTexture(t, APP.Renderer.NextAvailSlot);
    APP.Renderer.Vertices[APP.Renderer.VerticesCount + 0].Pos.x = r.x;
    APP.Renderer.Vertices[APP.Renderer.VerticesCount + 0].Pos.y = r.y;
    APP.Renderer.Vertices[APP.Renderer.VerticesCount + 0].TexCoords = Vec2Create(0.0f, 0.0f);
    APP.Renderer.Vertices[APP.Renderer.VerticesCount + 0].TexID = APP.Renderer.NextAvailSlot;

    APP.Renderer.Vertices[APP.Renderer.VerticesCount + 1].Pos.x = r.x + r.w;
    APP.Renderer.Vertices[APP.Renderer.VerticesCount + 1].Pos.y = r.y;
    APP.Renderer.Vertices[APP.Renderer.VerticesCount + 1].TexCoords = Vec2Create(1.0f, 0.0f);
    APP.Renderer.Vertices[APP.Renderer.VerticesCount + 1].TexID = APP.Renderer.NextAvailSlot;

    APP.Renderer.Vertices[APP.Renderer.VerticesCount + 2].Pos.x = r.x + r.w;
    APP.Renderer.Vertices[APP.Renderer.VerticesCount + 2].Pos.y = r.y + r.h;
    APP.Renderer.Vertices[APP.Renderer.VerticesCount + 2].TexCoords = Vec2Create(1.0f, 1.0f);
    APP.Renderer.Vertices[APP.Renderer.VerticesCount + 2].TexID = APP.Renderer.NextAvailSlot;

    APP.Renderer.Vertices[APP.Renderer.VerticesCount + 3].Pos.x = r.x;
    APP.Renderer.Vertices[APP.Renderer.VerticesCount + 3].Pos.y = r.y + r.h;
    APP.Renderer.Vertices[APP.Renderer.VerticesCount + 3].TexCoords = Vec2Create(0.0f, 1.0f);
    APP.Renderer.Vertices[APP.Renderer.VerticesCount + 3].TexID = APP.Renderer.NextAvailSlot;

    APP.Renderer.NextAvailSlot += 1;
    APP.Renderer.VerticesCount += 4;
}

IMAGE* LoadImage(const void* data, int width, int height)
{
    IMAGE* img = malloc(sizeof(IMAGE));
    img->Width = width;
    img->Height = height;
    img->Data = (void*) data;
    img->LoadedFromFile = false;
    return img;
}

IMAGE* LoadImageFromFile(const char* path, bool flip)
{
    stbi_set_flip_vertically_on_load(flip);
    int w, h;
    uint8_t* data = stbi_load(path, &w, &h, NULL, 4);
    IMAGE* img = LoadImage((void*)data, w, h);
    img->LoadedFromFile = true;
    return img;
}

RECTANGLE GetImageShape(const IMAGE* img)
{
    RECTANGLE r = {0};
    r.w = (float) img->Width;
    r.h = (float) img->Height;
    r.x = 0.0f;
    r.y = 0.0f;
    return r;
}

void DestroyImage(IMAGE* image)
{
    stbi_image_free(image->Data);
    free(image);
    image = NULL;
}

TEXTURE2D LoadTextureFromImage(const IMAGE* image)
{
    TEXTURE2D tex = hxglLoadTexture(image->Data, image->Width, image->Height, HXGL_LINEAR_MIPMAP_LINEAR);
    return tex;
}