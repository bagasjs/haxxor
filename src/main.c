#include <hxgl.h>
#include <haxxor.h>
#include <stdint.h>

int main(void)
{
	if(!InitHaxxor("My Window", 640.0f, 480.0f)) return -1;

	uint32_t vao = hxglLoadVertexArray();
	hxglEnableVertexArray(vao);

	float vertices[] = {
		0.5f, 0.5f,
		-0.5f, 0.5f,
		-0.5f, -0.5f,
	};

	uint32_t vbo = hxglLoadVertexBuffer(vertices, sizeof(vertices), false);
	hxglEnableVertexBuffer(vbo);
	hxglSetVertexAttribute(0, 2, HXGL_FLOAT, false, 2 * sizeof(float), (void*) 0);

	uint32_t elements[] = { 0, 1, 2 };
	uint32_t ibo = hxglLoadIndexBuffer(elements, sizeof(elements), false);
	hxglEnableIndexBuffer(ibo);

	while(!ShouldClose())
	{
		PollEvents();
		hxglDrawVertexArrayElements(0, 3, (void*)0);
		Update();
	}

	ShutHaxxor();
}