#include "GUI.h"

#include "menu_block.h"
#include "transform_window.h"
#include "list_window.h"
#include "file_explorer.h"
#include "params_window.h"
#include "console.h"

#include "camera.h"

#include "e_math.h"

#include "resource.h"


void GUI_Resize()
{
    MenuBlockResize();
    TransformWindowResize();
    ListWindowResize();
    ParamsWindowResize();
    FileExplorertWindowResize();
    ConsoleWindowResize();
}

void GUI_Init()
{
    MenuBlockInit();
    ListWindowInit();
    TransformWindowInit();
    ParamsWindowInit();
    FileExplorerWindowInit();
    ConsoleInit();

    setViewPos(-10, 10, -10);
}


void GUI_Update(float delta_time)
{
    MenuBlockUpdate(delta_time);
    ListWindowUpdate(delta_time);
    TransformWindowUpdate(delta_time);
    ParamsWindowUpdate();
    ConsoleUpdate(delta_time);
    FileExplorerWindowUpdate(delta_time);
}

void GUI_Draw()
{
    MenuBlockDraw();
    ParamsWindowDraw();
    TransformWindowDraw();
    ListWindowDraw();
    FileExplorerWindowDraw();
    ConsoleDraw();
}

void GUI_Destroy()
{
    MenuBlockDestroy();
    ListWindowDestroy();
    TransformWindowDestroy();
    FileExplorerWindowDestroy();
    ParamsWindowDestroy();
    ConsoleDestroy();
}
