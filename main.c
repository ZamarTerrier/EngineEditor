#include <stdio.h>

#include "engine.h"

#include "editor.h"

float dTime = 0.1;

void Run(){

    float want_fps = 60;

    double lastTime = 0;
    int nbFrames = 0;

    while(!EngineWindowIsClosed())
    {
        double startTime = EngineGetTime();
        EnginePoolEvents();
        Update(dTime);
        Draw();
        EngineLoop();
        double endTime = EngineGetTime();

        //Вычисление потенциала обновления кадров, для дальнейшего использования
        dTime = startTime - endTime;
        if(dTime < 0)
            dTime = -dTime;

        //Ограничение кадров
        int time_to_sleep = (1.f/want_fps - dTime);

        if (time_to_sleep > 0) {
            sleep(time_to_sleep);
        }
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
