#include "TestEngine.h"

int main(int, char**)
{
    TestEngine Engine;

    Engine.Init();

    // Main loop
    while (!glfwWindowShouldClose(Engine.WindowData.Window))
    {
        Engine.Update();
        Engine.Render();
    }

    Engine.Terminate();
    
    return 0;
}
