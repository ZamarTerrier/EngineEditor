#include "editor.h"

#include "GUI.h"

void Init()
{
    GUI_Init();
}

void Update()
{
    GUI_Update();
}

void Draw()
{
    GUI_Draw();
}

void Clean()
{
    GUI_Destroy();
}
