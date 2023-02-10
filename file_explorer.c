#include "file_explorer.h"

#include <dirent.h>

#include <e_widget.h>
#include <e_widget_text.h>
#include <e_widget_entry.h>
#include <e_widget_window.h>
#include <e_widget_scroll.h>
#include <e_widget_list.h>

#include <e_math.h>

#include "console.h"

#include "resource.h"

EWidgetList list_files;

callback_name c_funk;

char some_dir[] = "/home/ilia/";

void FileExplorertWindowResize()
{
    WidgetRecreate(&explorer_window.top);
}

void FileExplorerAccept(EWidget *widget, void *entry, EWidgetEntry *line)
{
    if(c_funk == NULL)
        return;

    char temp[256];

    ToolsAddStrings(temp, 256, some_dir, EntryWidgetGetText(line));

    if(opendir(temp) != 0)
    {
        ConsoleInputText("Ошибка : Это папка!\n");
        return;
    }

    c_funk(temp);

    EntryWidgetSetText(line, "");

    WindowWidgetHide(&explorer_window);
}

void FileExplorerClose(EWidget *widget, void *entry, EWidgetEntry *line)
{
    c_funk = NULL;

    EntryWidgetSetText(line, "");

    WindowWidgetHide(&explorer_window);
}

void FileExplorerSetCallback(callback_name some_funk)
{
    c_funk = some_funk;
}

void SomeExplober(EWidget* widget, float *valu, EWidgetList *list)
{
    float size = list->size * list->size_y * 2 - 560;

    float result = size * *valu;

    Transform2DSetPosition(list, 0, 0 - result);
}

void FileWasSellected(EWidget* widget, int id, EWidgetEntry *line)
{
    EWidgetButton *button = widget;

    char buff[256];

    memset(buff, 0, 256);

    TextWidgetGetText(&button->text, buff);

    EntryWidgetSetText(line, buff);
}

void LetsGoToPath(EWidget* widget, void *entry, EWidgetEntry *line)
{

    int count = list_files.size;
    while(list_files.size > 0){
        count = list_files.size;
        ListWidgetRemoveItem(&list_files, list_files.size - 1);
    }

    char *pather = EntryWidgetGetText(line);

    DIR *dir;

    dir = opendir(pather);

    if(!dir)
    {
        ConsoleInputText("Ошибка : Не правильный путь!\n");
        return;
    }

    struct dirent *dp;

    EWidgetButton *some_button;

    while ((dp = readdir(dir)) != NULL)
    {
        some_button = ListWidgetAddItem(&list_files, dp->d_name);
        WidgetConnect(some_button, GUI_TRIGGER_BUTTON_PRESS, FileWasSellected, entry);
    }
}

void FileExplorerWindowInit()
{
    vec2 size = { 600, 400};
    vec2 position = {100, 100};
    WindowWidgetInit(&explorer_window, "Проводник", size, NULL, position);
    explorer_window.resizeble = false;
    WindowWidgetHide(&explorer_window);

    EWidgetText *label1 = calloc(1, sizeof(EWidgetText));
    TextWidgetInit(label1, 9, NULL, &explorer_window);
    TextWidgetSetText(label1, "Расположение :");
    Transform2DSetPosition(label1, 10, 40);

    EWidgetEntry *pather = calloc(1, sizeof(EWidgetEntry));
    EntryWidgetInit(pather, 9, &explorer_window);
    Transform2DSetScale(pather, size.x - 240, 20);
    Transform2DSetPosition(pather, 250, 8);
    EntryWidgetSetText(pather, some_dir);

    EWidgetButton *goPath = calloc(1, sizeof(EWidgetButton));
    ButtonWidgetInit(goPath, "Перейти", &explorer_window);
    Transform2DSetScale(goPath, 90, 24);
    Transform2DSetPosition(goPath, (size.x - 100) * 2, 5);
    WidgetConnect(goPath, GUI_TRIGGER_BUTTON_PRESS, LetsGoToPath, pather);

    EWidgetScroll *scroller = calloc(1, sizeof(EWidgetScroll));
    ScrollWidgetInit(scroller, size.x, size.y - 120, NULL, &explorer_window);
    Transform2DSetPosition(scroller, 0, 60);

    ListWidgetInit(&list_files, size.x - 20, 30, scroller);

    EWidgetText *label2 = calloc(1, sizeof(EWidgetText));
    TextWidgetInit(label2, 9, NULL, &explorer_window);
    TextWidgetSetText(label2, "Имя файла :");
    Transform2DSetPosition(label2, 0, (size.y - 65) * 2);

    EWidgetEntry *entry = calloc(1, sizeof(EWidgetEntry));
    EntryWidgetInit(entry, 9, &explorer_window);
    Transform2DSetScale(entry, size.x - 100, 20);
    Transform2DSetPosition(entry, 180, (size.y - 78) * 2);

    EWidgetButton *cancel = calloc(1, sizeof(EWidgetButton));
    ButtonWidgetInit(cancel, "Отмена", &explorer_window);
    Transform2DSetScale(cancel, 70, 30);
    Transform2DSetPosition(cancel, (size.x - 190) * 2, (size.y - 45) * 2);
    WidgetConnect(cancel, GUI_TRIGGER_BUTTON_PRESS, FileExplorerClose, entry);

    EWidgetButton *accept = calloc(1, sizeof(EWidgetButton));
    ButtonWidgetInit(accept, "Применить", &explorer_window);
    Transform2DSetScale(accept, 90, 30);
    Transform2DSetPosition(accept, (size.x - 100) * 2, (size.y - 45) * 2);
    WidgetConnect(accept, GUI_TRIGGER_BUTTON_PRESS, FileExplorerAccept, entry);


    WidgetConnect(scroller, GUI_TRIGGER_SCROLL_CHANGE, SomeExplober, &list_files);

    DIR *dir;

    dir = opendir(some_dir);

    struct dirent *dp;

    EWidgetButton *some_button;

    while ((dp = readdir(dir)) != NULL)
    {
        some_button = ListWidgetAddItem(&list_files, dp->d_name);
        WidgetConnect(some_button, GUI_TRIGGER_BUTTON_PRESS, FileWasSellected, entry);
    }

    ScrollWidgetUpdate(scroller, &list_files);

    closedir(dir);
}

void FileExplorerWindowUpdate(float delta_time)
{
    WindowWidgetUpdate(&explorer_window);

}

void FileExplorerWindowDraw()
{
    WindowWidgetDraw(&explorer_window);
}

void FileExplorerWindowDestroy()
{
    WindowWidgetDestroy(&explorer_window);
}
