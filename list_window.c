#include "list_window.h"

#include <stdlib.h>

#include <gameObject.h>
#include <primitiveObject.h>

#include <e_widget_button.h>
#include <e_widget_window.h>
#include <e_widget_scroll.h>
#include <e_widget_list.h>

#include <e_resource_data.h>

#include "transform_window.h"

#include "resource.h"

EWidgetScroll list_scroll;
EWidgetList list_objects;

DrawParam list_dParam;

int ListWindowSomeExplober(EWidget* widget, float *valu, EWidgetList *list)
{
    float size = list->size * list->size_y * 2 - 510;

    float result = size * *valu;

    Transform2DSetPosition(list, 0, 0 - result);

    return 0;
}

void ListWindowClickList(EWidget *widget, int id, void *arg)
{
    TransformWindowSetValues(Transform3DGetPosition(objects[id]), Transform3DGetRotate(objects[id]), Transform3DGetScale(objects[id]));

    curr_object = id;
}

void ListWindowResize()
{
    Transform2DSetPosition(&list_window.top, (WIDTH * 2) - 200, 970);
    WidgetRecreate(&list_window.top);
}

void ListWindowRemoveItem(EWidget *widget, void *entry, void *arg)
{
    if(curr_object == -1)
        return;

    GameObjectDestroy(objects[curr_object]);
    free(objects[curr_object]);

    PrimitiveObject **new_buffer = calloc(num_objects, sizeof(PrimitiveObject *));
    PrimitiveObject **old_buffer = objects;

    int iter = 0;
    for(int i=0;i < num_objects;i++)
    {
        if(objects[curr_object] != objects[i])
        {
            new_buffer[iter] = old_buffer[i];
            iter++;
        }
    }

    ListWidgetRemoveItem(&list_objects, curr_object);
    ScrollWidgetUpdate(&list_scroll, &list_objects);

    num_objects --;

    free(objects);
    objects = new_buffer;

    curr_object = -1;
}

void ListWindowInit()
{
    memset(&list_dParam, 0, sizeof(DrawParam));

    list_dParam.render = &render_window;

    char img_path[256];

    vec2 size = { 150, 300};
    vec2 position = {WIDTH * 2 - 300, 970};
    WindowWidgetInit(&list_window, "Список", size, &list_dParam, position);
    list_window.resizeble = false;

    EWidgetButton *del_button = calloc(1, sizeof(EWidgetButton));
    ButtonWidgetInit(del_button, "", &list_dParam, &list_window);
    ButtonWidgetSetColor(del_button, 0.4f, 0.4f, 0.4f);
    ToolsAddStrings(img_path, 256, editor_path, "textures/trash.png");
    ButtonWidgetSetImage(del_button, img_path, &list_dParam);
    del_button->widget.widget_flags |= ENGINE_FLAG_WIDGET_ALLOCATED;
    Transform2DSetScale(del_button, 30, 30);
    WidgetConnect(del_button, ENGINE_WIDGET_TRIGGER_BUTTON_PRESS, ListWindowRemoveItem, NULL);

    ScrollWidgetInit(&list_scroll, size.x, size.y - 45, &list_dParam, &list_window);
    Transform2DSetPosition(&list_scroll, 0, 60);
    WidgetConnect(&list_scroll, ENGINE_WIDGET_TRIGGER_SCROLL_CHANGE, ListWindowSomeExplober, &list_objects);

    ListWidgetInit(&list_objects, size.x - 20, 20, &list_dParam, &list_scroll);
    WidgetConnect(&list_objects, ENGINE_WIDGET_TRIGGER_LIST_PRESS_ITEM, ListWindowClickList, NULL);

    ScrollWidgetUpdate(&list_scroll, &list_objects);
}

void ListWindowUpdate(float delta_time)
{
    WindowWidgetUpdate(&list_window);

    while(list_objects.size != num_objects)
    {
        char buff[10];
        sprintf(buff, "%i",list_objects.size + 1);
        ListWidgetAddItem(&list_objects, buff, &list_dParam);

        ScrollWidgetUpdate(&list_scroll, &list_objects);
    }
}

void ListWindowDraw()
{
    WindowWidgetDraw(&list_window);
}

void ListWindowDestroy()
{
    WindowWidgetDestroy(&list_window);
}
