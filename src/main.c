#include <haxxor.h>
#include <hxgl.h>

int main(void)
{
    if(!InitHaxxor("My Window", 640.0f, 480.0f)) return -1;
    while(!ShouldClose())
    {
        PollEvents();
        Update();
    }

    ShutHaxxor();
}