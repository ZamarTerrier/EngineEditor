#include "GUI.h"

#include "menu_block.h"
#include "transform_window.h"
#include "list_window.h"

#include "camera.h"

#include "e_math.h"

#include "resource.h"


void GUI_Resize()
{
    MenuBlockResize();
}

void GUI_Init()
{
    MenuBlockInit();
    ListWindowInit();
    TransformWindowInit();
    setViewPos(-10, 10, -10);
}


void GUI_Update(float delta_time)
{
    MenuBlockUpdate(delta_time);
    ListWindowUpdate(delta_time);
    TransformWindowUpdate(delta_time);
}

void GUI_Draw()
{
    MenuBlockDraw();
    TransformWindowDraw();
    ListWindowDraw();
}

void GUI_Destroy()
{
    MenuBlockDestroy();
    ListWindowDestroy();
    TransformWindowDestroy();
}
