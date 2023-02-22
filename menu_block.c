#include "menu_block.h"

#include <engine.h>

#include "console.h"

#include "transform_window.h"
#include "file_explorer.h"

#include <e_widget.h>
#include <e_widget_window.h>
#include <e_widget_top_menu.h>

#include <primitiveObject.h>

#include <camera.h>

#include <e_math.h>

#include <resource.h>

EWidgetTopMenu menu;

void SaveFileFunc(const char* name)
{

    FILE *file = fopen(name, "w");

    char str_err[1024];

    if(!file){
        ConsoleInputText("Ошибка записи файла: Ошибка открытия файла!\n");
        return;
    }

    char *temp = "Hello world!";

    fwrite(temp, sizeof(char), strlen(temp), file);

    fclose(file);

    return;
}

void OpenFileFunc(const char* name)
{

    if(strlen(name) == 0)
    {
        ConsoleInputText("Ошибка чтения файла : Нет имени!\n");
        return;
    }


    char buff[256];

    FILE *file = fopen(name, "r");

    char str_err[1024];

    if(!file){
        sprintf(str_err, "Ошибка чтения файла : Ошибка открытия файла \"%s\" !\n", name);
        ConsoleInputText(str_err);
        return;
    }

    fread(buff, sizeof(char), 256, file);

    fclose(file);

    return;
}

void ShowTransformMenu(EWidget *widget, int id, void *args)
{
    WindowWidgetShow(&transform_window);
}

void ShowListMenu(EWidget *widget, int id, void *args)
{
    WindowWidgetShow(&list_window);
}

void MakeObject(int id)
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
    ToolsAddStrings(dParam.diffuse, 256, editor_path, "textures/texture.png");
    objects[num_objects - 1] = calloc(1, sizeof(PrimitiveObject));
    PrimitiveObjectInit(objects[num_objects - 1], dParam, type, some_params);

    GameObject3D *some_obj = objects[num_objects - 1];
    some_obj->wired = true;

    vec3 dir = v3_norm(getViewRotation());
    vec3 pos = v3_add(getViewPos(), v3_muls(dir, -5));

    curr_object = num_objects - 1;

    vec3 null_vector = { 0, 0, 0};
    vec3 eden_vector = { 1, 1, 1};
    TransformWindowSetValues(pos, null_vector, eden_vector);
}

int ExplorerMenuSomeDo(EWidget *widget, MenuData *data, void *args)
{
    if(data->num_menu == 0)
    {
        switch(data->elem_id)
        {
                //Открыть
            case 0:
                FileExplorerSetCallback((void *)OpenFileFunc);
                WindowWidgetShow(&explorer_window);
                break;
                //Сохранить как
            case 2:
                FileExplorerSetCallback((void *)SaveFileFunc);
                WindowWidgetShow(&explorer_window);
                break;
        }
    }else if(data->num_menu == 2)
        MakeObject(data->elem_id);
    else if(data->num_menu == 3)
    {
        switch(data->elem_id)
        {
                //Окно трансформации
            case 0:
                WindowWidgetShow(&transform_window);
                break;
                //Окно элементов
            case 1:
                WindowWidgetShow(&list_window);
                break;
        }
    }

    return 0;
}

void MenuBlockResize()
{
    TopMenuWidgetResize(&menu);
    WidgetRecreate(&menu.widget);
}

void MenuBlockFocus()
{
    EngineFixedCursorCenter();
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
    TopMenuWidgetInit(&menu, NULL);
    WidgetConnect(&menu.widget, ENGINE_WIDGET_TRIGGER_MOUSE_PRESS, MenuBlockFocus, NULL);
    WidgetConnect(&menu.widget, ENGINE_WIDGET_TRIGGER_MOUSE_RELEASE, MenuBlockUnFocus, NULL);
    int num = TopMenuWidgetAddMenu(&menu, "Файл");
    WidgetConnect(&menu, ENGINE_WIDGET_TRIGGER_MENU_PRESS_ITEM, ExplorerMenuSomeDo, NULL);
    TopMenuWidgetAddItem(&menu, num, "Открыть");
    TopMenuWidgetAddItem(&menu, num, "Сохранить");
    TopMenuWidgetAddItem(&menu, num, "Сохранить как");
    TopMenuWidgetAddItem(&menu, num, "Запилить");
    TopMenuWidgetAddItem(&menu, num, "Выход");

    num = TopMenuWidgetAddMenu(&menu, "Настройки");
    TopMenuWidgetAddItem(&menu, num, "Отменить");
    TopMenuWidgetAddItem(&menu, num, "Повторить");
    TopMenuWidgetAddItem(&menu, num, "Параметры");

    num = TopMenuWidgetAddMenu(&menu, "Добавить");
    TopMenuWidgetAddItem(&menu, num, "Квадрат");
    TopMenuWidgetAddItem(&menu, num, "Куб");
    TopMenuWidgetAddItem(&menu, num, "Сфера");
    TopMenuWidgetAddItem(&menu, num, "Плоскость");
    TopMenuWidgetAddItem(&menu, num, "Местность");

    num = TopMenuWidgetAddMenu(&menu, "Окна");
    TopMenuWidgetAddItem(&menu, num, "Объект");
    TopMenuWidgetAddItem(&menu, num, "Список");
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
