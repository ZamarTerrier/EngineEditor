#include "file_explorer.h"

#include <dirent.h>

#include <e_widget.h>
#include <e_widget_window.h>
#include <e_widget_list.h>

#include "resource.h"

void FileExplorertWindowResize()
{
    WidgetRecreate(&explorer_window.top);
}

void FileExplorerWindowInit()
{
    vec2 size = { 600, 300};
    vec2 position = {100, 100};
    WindowWidgetInit(&explorer_window, "Проводник", size, NULL, position);
    explorer_window.resizeble = false;
    WindowWidgetHide(&explorer_window);

    EWidgetList *list_files = calloc(1, sizeof(EWidgetList));
    ListWidgetInit(list_files, 600, 30, &explorer_window);

    DIR *dir;

    dir = opendir("/home/ilia");

    struct dirent *dp;

    while ((dp = readdir(dir)) != NULL)
    {
        ListWidgetAddItem(list_files, dp->d_name);
    }

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
