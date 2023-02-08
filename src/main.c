#include <hxgl.h>
#include <haxxor.h>
#include <stdint.h>

int main(void)
{
	if(!InitHaxxor("My Window", 640.0f, 480.0f)) return -1;

	RECTANGLE player = { 0.0f, 0.0f, 0.5f, 0.5f };
	RECTANGLE enemy = { 0.0f, 0.0f, -0.5f, -0.5f };
	const COLOR RED = { 255, 0, 0, 255 };
	const COLOR BLUE = { 0, 0, 255, 255 };

	while(!ShouldClose())
	{
		PollEvents();
		BeginDraw();
		DrawRectangle(player, BLUE);
		DrawRectangle(enemy, RED);
		EndDraw();
	}

	ShutHaxxor();
}