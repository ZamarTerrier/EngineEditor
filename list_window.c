#include "list_window.h"

#include <stdlib.h>

#include <e_widget_window.h>
#include <e_widget_scroll.h>
#include <e_widget_list.h>

#include "transform_window.h"

#include "resource.h"

EWidgetScroll list_scroll;
EWidgetList list_objects;

int ListWindowSomeExplober(EWidget* widget, float *valu, EWidgetList *list)
{
    float size = list->size * list->size_y * 2 - 560;

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

void ListWindowInit()
{
    vec2 size = { 100, 300};
    vec2 position = {WIDTH * 2 - 200, 970};
    WindowWidgetInit(&list_window, "Список", size, NULL, position);
    list_window.resizeble = false;

    ScrollWidgetInit(&list_scroll, size.x, size.y - 20, NULL, &list_window);
    WidgetConnect(&list_scroll, GUI_TRIGGER_SCROLL_CHANGE, ListWindowSomeExplober, &list_objects);

    ListWidgetInit(&list_objects, size.x - 20, 20, &list_scroll);
    WidgetConnect(&list_objects, GUI_TRIGGER_LIST_PRESS_ITEM, ListWindowClickList, NULL);

    ScrollWidgetUpdate(&list_scroll, &list_objects);
}

void ListWindowUpdate(float delta_time)
{
    WindowWidgetUpdate(&list_window);

    while(list_objects.size != num_objects)
    {
        char buff[10];
        sprintf(buff, "%i",list_objects.size + 1);
        ListWidgetAddItem(&list_objects, buff);

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
