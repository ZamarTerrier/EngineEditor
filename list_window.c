#include "list_window.h"

#include <stdlib.h>

#include "e_widget_window.h"
#include "e_widget_list.h"

#include "transform_window.h"

#include "resource.h"

EWidgetList list_objects;

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

void ListWindowInit()
{
    vec2 size = { 100, 300};
    vec2 position = {WIDTH * 2 - 200, 970};
    WindowWidgetInit(&list_window, "Список", size, NULL, position);
    list_window.resizeble = false;

    ListWidgetInit(&list_objects, 100, 20, &list_window);
    WidgetConnect(&list_objects, GUI_TRIGGER_LIST_PRESS_ITEM, ListWindowClickList, NULL);
}

void ListWindowUpdate(float delta_time)
{
    WindowWidgetUpdate(&list_window);

    while(list_objects.size != num_objects)
    {
        char buff[10];
        sprintf(buff, "%i",list_objects.size + 1);
        ListWidgetAddItem(&list_objects, buff);
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
