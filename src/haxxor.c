#include "haxxor.h"
#include <hxgl.h>
#include <GLFW/glfw3.h>

typedef struct Application {
    bool Initialized;
    struct {
        GLFWwindow* Handle;
    } Surface;
    struct {
        uint32_t vao, vbo, ibo;
        uint32_t Elements[MAXIMUM_ELEMENTS];
        uint32_t ElementsCount;
    } Renderer;
} Application;

static Application APP = {0};

bool InitHaxxor(const char* name, float width, float height)
{
    if(APP.Initialized) return false; // Haxxor has been initialized
    if(!glfwInit()) return false; // Failed to initialize glfw
    APP.Surface.Handle = glfwCreateWindow((int)width, (int)height, name, NULL, NULL);
    if(APP.Surface.Handle == NULL) return false; // Failed to create window
    glfwMakeContextCurrent(APP.Surface.Handle);
    hxglLoadExtension(glfwGetProcAddress);

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

void Update()
{
    glfwSwapBuffers(APP.Surface.Handle);
    hxglClear();
}