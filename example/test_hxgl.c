#include <hxgl.h>
#include <haxxor.h>
#include <stdint.h>

int main(void)
{
	if(!InitHaxxor("My Window", 640.0f, 480.0f)) return -1;

	uint32_t vao = hxglLoadVertexArray();
	hxglEnableVertexArray(vao);

	float vertices[] = {
        -0.5f,  0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
	};

	uint32_t vbo = hxglLoadVertexBuffer(vertices, sizeof(vertices), false);
	hxglEnableVertexBuffer(vbo);
	hxglSetVertexAttribute(0, 3, HXGL_FLOAT, false, 3 * sizeof(float), (void*) 0);

	uint32_t elements[] = { 0, 1, 2, 2, 3, 0 };
	uint32_t ibo = hxglLoadIndexBuffer(elements, sizeof(elements), false);
	hxglEnableIndexBuffer(ibo);

	while(!ShouldClose())
	{
		PollEvents();
		hxglDrawVertexArrayElements(0, 6, 0);
		SwapBuffers();
		hxglClear();
	}

	hxglDropIndexBuffer(ibo);
	hxglDropVertexBuffer(vbo);
	hxglDropVertexArray(vao);

	ShutHaxxor();
}