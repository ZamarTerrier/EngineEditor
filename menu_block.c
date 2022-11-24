#include "menu_block.h"

#include "engine.h"

#include "transform_window.h"

#include "e_widget.h"
#include "e_widget_window.h"
#include "e_widget_top_menu.h"

#include "primitiveObject.h"

#include "camera.h"

#include "e_math.h"

#include "resource.h"

EWidgetTopMenu menu;

void ShowTransformMenu(EWidget *widget, void *entry, void *args)
{
    WindowWidgetShow(&transform_window);
}

void ShowListMenu(EWidget *widget, void *entry, void *args)
{
    WindowWidgetShow(&list_window);
}

void MenuBlockResize()
{
    TopMenuWidgetResize(&menu);
}

void MakeObject(EWidget *widget, void *entry, int id)
{

    char type;

    void *some_params = NULL;

    PlaneParam pParam;
    SphereParam sParam;
    TerrainParam tParam;

    switch(id)
    {
        case 0:
            type = ENGINE_PRIMITIVE3D_QUAD;
            break;
        case 1:
            type = ENGINE_PRIMITIVE3D_CUBE;
            break;
        case 2:
            type = ENGINE_PRIMITIVE3D_SPHERE;
            sParam.radius = 1;
            sParam.sectorCount = 10;
            sParam.stackCount = 10;
            some_params = &sParam;
            break;
        case 3:
            type = ENGINE_PRIMITIVE3D_PLANE;
            pParam.sectorCount = 20;
            pParam.stackCount = 20;
            some_params = &pParam;
            break;
        case 4:
            type = ENGINE_PRIMITIVE3D_TERRAIN;
            tParam.cell_step = 4;
            tParam.colmns = 20;
            tParam.rows = 20;
            some_params = &tParam;
            break;
        default:
            return;

    }

    num_objects ++;

    objects = realloc(objects, num_objects * sizeof(PrimitiveObject*));

    DrawParam dParam;
    memset(&dParam, 0, sizeof(DrawParam));
    ToolsAddStrings(dParam.diffuse, 256, path, "textures/texture.png");
    objects[num_objects - 1] = calloc(1, sizeof(PrimitiveObject));
    PrimitiveObjectInit(objects[num_objects - 1], dParam, type, some_params);


    vec3 dir = v3_sub(getViewPos(), Transform3DGetPosition(objects[num_objects - 1]));
    dir = v3_norm(dir);
    setViewRotation(dir.x * 180, dir.y * 180, dir.z * 180);

    curr_object = num_objects - 1;

    vec3 null_vector = { 0, 0, 0};
    vec3 eden_vector = { 1, 1, 1};
    TransformWindowSetValues(null_vector, null_vector, eden_vector);
}

void MenuBlockFocus()
{
    EngineHideCursor(1);
    editor_focus = true;
}

void MenuBlockUnFocus()
{
    editor_focus = false;
    EngineHideCursor(2);
}


void MenuBlockInit()
{
    EWidgetButton *button;

    TopMenuWidgetInit(&menu, NULL);
    WidgetConnect(&menu.widget, GUI_TRIGGER_WIDGET_FOCUS, MenuBlockFocus, NULL);
    WidgetConnect(&menu.widget, GUI_TRIGGER_MOUSE_PRESS, MenuBlockFocus, NULL);
    WidgetConnect(&menu.widget, GUI_TRIGGER_WIDGET_UNFOCUS, MenuBlockUnFocus, NULL);
    int num = TopMenuWidgetAddMenu(&menu, "Файл");
    TopMenuWidgetAddItem(&menu, num, "Открыть");
    TopMenuWidgetAddItem(&menu, num, "Закрыть");
    TopMenuWidgetAddItem(&menu, num, "Запилить");

    num = TopMenuWidgetAddMenu(&menu, "Добавить");
    button = TopMenuWidgetAddItem(&menu, num, "Квадрат");
    WidgetConnect(button, GUI_TRIGGER_BUTTON_PRESS, MakeObject, 0);
    button = TopMenuWidgetAddItem(&menu, num, "Куб");
    WidgetConnect(button, GUI_TRIGGER_BUTTON_PRESS, MakeObject, 1);
    button = TopMenuWidgetAddItem(&menu, num, "Сфера");
    WidgetConnect(button, GUI_TRIGGER_BUTTON_PRESS, MakeObject, 2);
    button = TopMenuWidgetAddItem(&menu, num, "Плоскость");
    WidgetConnect(button, GUI_TRIGGER_BUTTON_PRESS, MakeObject, 3);
    button = TopMenuWidgetAddItem(&menu, num, "Местность");
    WidgetConnect(button, GUI_TRIGGER_BUTTON_PRESS, MakeObject, 4);

    num = TopMenuWidgetAddMenu(&menu, "Окна");
    button = TopMenuWidgetAddItem(&menu, num, "Объект");
    WidgetConnect(button, GUI_TRIGGER_BUTTON_PRESS, ShowTransformMenu, NULL);
    button = TopMenuWidgetAddItem(&menu, num, "Список");
    WidgetConnect(button, GUI_TRIGGER_BUTTON_PRESS, ShowListMenu, NULL);
}

void MenuBlockUpdate(float delta_time)
{
    WidgetEventsPipe(&menu);
}

void MenuBlockDraw()
{
    WidgetDraw(&menu);
}

void MenuBlockDestroy()
{
    WidgetDestroy(&menu);
}
