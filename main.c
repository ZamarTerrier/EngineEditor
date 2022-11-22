#include <stdio.h>

#include "engine.h"

#include "editor.h"

void Run(){

    while(!EngineWindowIsClosed())
    {
        EnginePoolEvents();
        Update();
        Draw();
        EngineLoop();
    }

    EngineDeviceWaitIdle();
}

int main()
{
    EngineInitSystem(1024, 800, "Editor");

    Init();

    Run();

    Clean();

    EngineCleanUp();

    return 0;
}
