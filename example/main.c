#include <hxgl.h>
#include <haxxor.h>
#include <stdint.h>

int main(void)
{
	const COLOR RED = { 255, 0, 0, 255 };
	const COLOR BLUE = { 0, 0, 255, 255 };
	const float SCREEN_WIDTH = 640.0f;
	const float SCREEN_HEIGHT = 480.0f;
	if(!InitHaxxor("My Window", SCREEN_WIDTH, SCREEN_HEIGHT)) return -1;

	RECTANGLE player = { 0.0f, 0.0f, 100.0f, 100.0f };
	RECTANGLE enemy = { 100.0f, 0.0f, 100.0f, 100.0f };
	IMAGE* img = LoadImageFromFile("res/ely.jpg", false);
	TEXTURE2D ely_t = LoadTextureFromImage(img);

	// Resizing image
	float target_width = SCREEN_WIDTH;
	RECTANGLE ely_s = GetImageShape(img);
	ely_s.w = target_width;
	ely_s.h = ely_s.w / ely_s.h * target_width;

	while(!ShouldClose())
	{
		PollEvents();
		BeginDraw();
		DrawRectangle(player, BLUE);
		DrawRectangle(enemy, RED);
		DrawRectangleTex(ely_s, ely_t);
		EndDraw();
	}

	DestroyImage(img);

	ShutHaxxor();
}