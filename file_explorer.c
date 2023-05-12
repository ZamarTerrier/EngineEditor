#include "file_explorer.h"

#include <dirent.h>

#include <engine.h>

#include <e_widget.h>
#include <e_widget_text.h>
#include <e_widget_entry.h>
#include <e_widget_window.h>
#include <e_widget_scroll.h>
#include <e_widget_list.h>

#include "spriteObject.h"

#include <e_math.h>

#include "console.h"

#include "resource.h"

typedef struct{
    char path[10][256];
    int size;
}path_struct;

EWidgetList list_files;
EWidgetEntry name_entry, name_path;
EWidgetText path_label1, name_label;
EWidgetButton cancel_button, accept_button, go_path_button, up_folder;
EWidgetScroll files_scroll;

callback_name c_funk;

path_struct my_path;

DrawParam exploer_dParam;

int clean_list = 0;

char some_dir[] = "/home/ilia/";

void ParcePathForStruct(char *path, path_struct *some_struct)
{
    char some_buff[256];

    memset(some_struct, 0, sizeof(path_struct));
    memset(some_buff, 0, 256);

    char *pointer = path, *next_char;

    while(*pointer !=0 && (*pointer == ' ' || *pointer == '/'))
        pointer++;

    int iter = 0, dirty = 0;
    while(*pointer != 0)
    {

        if(*pointer != '/')
        {
            some_buff[iter] = *pointer;

            dirty = 1;

            iter++;
        }else{
            memcpy(some_struct->path[some_struct->size], some_buff, strlen(some_buff));
            memset(some_buff, 0, 256);

            iter = dirty = 0;

            some_struct->size++;
        }

        next_char = pointer + 1;

        if(*next_char == 0 && dirty)
        {
            memcpy(some_struct->path[some_struct->size], some_buff, strlen(some_buff));
            memset(some_buff, 0, 256);

            iter = 0;

            some_struct->size++;
        }


        pointer ++;
    }
}

void ParceStructToPath(char *dest, uint32_t dest_len, path_struct *some_struct)
{
    int some_size = some_struct->size, some_len = 0;
    for(int i=0;i < some_size;i++)
    {
        dest[strlen(dest)] = '/';
        ToolsAddStrings(dest, dest_len, dest, some_struct->path[i]);

    }
}

void FileExplorertWindowResize()
{
    WidgetRecreate(&explorer_window.top);
}

int FileExplorerAccept(EWidget *widget, void *entry, void *arg)
{
    if(c_funk == NULL)
        return;

    char temp[2048];
    char curr_dir[1024];

    memset(temp, 0, 2048);
    memset(curr_dir, 0, 1024);

    ParceStructToPath(curr_dir, 1024, &my_path);

    ToolsAddStrings(curr_dir, 2048, curr_dir, "/");
    ToolsAddStrings(temp, 2048, curr_dir, EntryWidgetGetText(&name_entry));

    if(opendir(temp) != 0)
    {
        ConsoleInputText("Ошибка : Это папка!\n");
        return;
    }

    c_funk(temp);

    EntryWidgetSetText(&name_entry, "");

    WindowWidgetHide(&explorer_window);

    return 0;
}

int FileExplorerClose(EWidget *widget, void *entry, void *arg)
{
    c_funk = NULL;

    EntryWidgetSetText(&name_entry, "");

    WindowWidgetHide(&explorer_window);

    return 0;
}

void FileExplorerSetCallback(callback_name some_funk)
{
    c_funk = some_funk;
}

int SomeExplober(EWidget* widget, float *valu, EWidgetList *list)
{
    float size = list->size * list->size_y * 2 - 560;

    float result = size * *valu;

    Transform2DSetPosition(list, 0, 0 - result);

    return 0;
}

int FileWasSellected(EWidget* widget, int id, EWidgetEntry *line)
{
    EWidgetList *some_list = widget;

    ChildStack *child = WidgetFindChild(some_list, id);

    EWidgetButton *button = child->node;

    char buff[256];

    memset(buff, 0, 256);

    TextWidgetGetText(&button->text, buff);

    char temp[1024];
    ParceStructToPath(temp, 1024, &my_path);
    ToolsAddStrings(temp, 1024, temp, "/");
    ToolsAddStrings(temp, 1024, temp, buff);

    DIR *dir = opendir(temp);

    if(dir)
    {
        closedir(dir);

        while(list_files.size > 0)
            ListWidgetRemoveItem(&list_files, list_files.size - 1);

        ParcePathForStruct(temp, &my_path);

        FillListCurrPath(temp, &list_files);

        EntryWidgetSetText(&name_path, temp);

        return -1;
    }

    closedir(dir);

    EntryWidgetSetText(line, buff);

    return 0;
}

void FillListCurrPath(char *path, EWidgetList *list)
{
    DIR *dir;

    dir = opendir(path);

    struct dirent *dp;

    EWidgetButton *some_button;

    char temp[1024];

    while ((dp = readdir(dir)) != NULL)
    {
        memset(temp, 0, 1024);

        if(dp->d_name[0] == '.' && dp->d_name[1] == 0)
            continue;
        else if(dp->d_name[0] == '.' && dp->d_name[1] == '.')
                continue;

        some_button = ListWidgetAddItem(list, dp->d_name, &exploer_dParam);

        ToolsAddStrings(temp, 1024, path, "/");
        ToolsAddStrings(temp, 1024, temp, dp->d_name);

        if(!opendir(temp))
            ToolsAddStrings(temp, 1024, editor_path, "textures/file.png");
        else
            ToolsAddStrings(temp, 1024, editor_path, "textures/folder.png");

        ButtonWidgetSetImage(some_button, temp, &exploer_dParam);
    }

    closedir(dir);

    ScrollWidgetUpdate(&files_scroll, &list_files);
}

int LetsGoToPath(EWidget* widget, void *entry, void *arg)
{

    while(list_files.size > 0){
        ListWidgetRemoveItem(&list_files, list_files.size - 1);
    }

    char *pather = EntryWidgetGetText(&name_path);

    DIR *dir;

    dir = opendir(pather);

    if(!dir)
    {
        ConsoleInputText("Ошибка : Не правильный путь!\n");
        return;
    }

    closedir(dir);

    ParcePathForStruct(pather, &my_path);

    FillListCurrPath(pather, &list_files);

    return 0;
}

int FileExplorerOnOpen(EWidget* widget, void *entry, void *arg)
{
    char path[2048];
    memset(path, 0, 2048);

    while(list_files.size > 0){
        ListWidgetRemoveItem(&list_files, list_files.size - 1);
    }

    ParceStructToPath(path, 2048, &my_path);

    EntryWidgetSetText(&name_path, path);

    FillListCurrPath(path, &list_files);

    return 0;
}

int FileExplorerOnClose(EWidget* widget, void *entry, void *arg)
{

    while(list_files.size > 0){
        ListWidgetRemoveItem(&list_files, list_files.size - 1);
    }

    EntryWidgetSetText(&name_path, "");

    EntryWidgetSetText(&name_entry, "");

    return 0;
}

int FileExplorerUpFolder(EWidget *widget, void *entry, void *arg)
{

    if(my_path.size <= 1)
    {
        ConsoleInputText("Ошибка : Это корневое расположение в файловой системе!\n");
        return 0;
    }

    while(list_files.size > 0){
        ListWidgetRemoveItem(&list_files, list_files.size - 1);
    }

    memset(my_path.path[my_path.size - 1], 0, 256);
    my_path.size --;

    char path[2048];

    ParceStructToPath(path, 2048, &my_path);

    EntryWidgetSetText(&name_path, path);

    FillListCurrPath(path, &list_files);

    return 0;
}

void FileExplorerWindowInit()
{

    memset(&exploer_dParam, 0, sizeof(DrawParam));
    exploer_dParam.render = &render_window;

    char temp[2048];

    vec2 size = { 600, 400};
    vec2 position = {100, 100};
    WindowWidgetInit(&explorer_window, "Проводник", size, &exploer_dParam, position);
    explorer_window.resizeble = false;
    WindowWidgetHide(&explorer_window);
    WidgetConnect(&explorer_window, ENGINE_WIDGET_TRIGGER_WINDOW_OPEN, FileExplorerOnOpen, NULL);
    WidgetConnect(&explorer_window, ENGINE_WIDGET_TRIGGER_WINDOW_CLOSE, FileExplorerOnClose, NULL);

    ToolsAddStrings(temp, 1024, editor_path, "textures/folder_up.png");
    ButtonWidgetInit(&up_folder, "", &exploer_dParam, &explorer_window);
    Transform2DSetScale(&up_folder, 30, 25);
    ButtonWidgetSetImage(&up_folder, temp, &exploer_dParam);
    WidgetConnect(&up_folder, ENGINE_WIDGET_TRIGGER_BUTTON_PRESS, FileExplorerUpFolder, NULL);
    ButtonWidgetSetColor(&up_folder, 0.35f, 0.35f, 0.35f);
    up_folder.widget.color.y = 0.35f;
    up_folder.widget.color.z = 0.35f;

    TextWidgetInitDefault(&path_label1, 9, &exploer_dParam, &explorer_window);
    TextWidgetSetText(&path_label1, "Расположение :");
    Transform2DSetPosition(&path_label1, 70, 40);

    EntryWidgetInit(&name_path, 9, &exploer_dParam, &explorer_window);
    Transform2DSetScale(&name_path, size.x - 300, 20);
    Transform2DSetPosition(&name_path, 320, 8);
    EntryWidgetSetText(&name_path, some_dir);

    ButtonWidgetInit(&go_path_button, "Перейти", &exploer_dParam, &explorer_window);
    Transform2DSetScale(&go_path_button, 90, 24);
    Transform2DSetPosition(&go_path_button, (size.x - 100) * 2, 5);
    WidgetConnect(&go_path_button, ENGINE_WIDGET_TRIGGER_BUTTON_PRESS, LetsGoToPath, NULL);

    ScrollWidgetInit(&files_scroll, size.x, size.y - 120, &exploer_dParam, &explorer_window);
    Transform2DSetPosition(&files_scroll, 0, 60);
    WidgetConnect(&files_scroll, ENGINE_WIDGET_TRIGGER_SCROLL_CHANGE, SomeExplober, &list_files);

    ListWidgetInit(&list_files, size.x - 20, 30, &exploer_dParam, &files_scroll);
    WidgetConnect(&list_files, ENGINE_WIDGET_TRIGGER_LIST_PRESS_ITEM, FileWasSellected, &name_entry);

    TextWidgetInitDefault(&name_label, 9, &exploer_dParam, &explorer_window);
    TextWidgetSetText(&name_label, "Имя файла :");
    Transform2DSetPosition(&name_label, 0, (size.y - 65) * 2);

    EntryWidgetInit(&name_entry, 9, &exploer_dParam, &explorer_window);
    Transform2DSetScale(&name_entry, size.x - 100, 20);
    Transform2DSetPosition(&name_entry, 180, (size.y - 78) * 2);

    ButtonWidgetInit(&cancel_button, "Отмена", &exploer_dParam, &explorer_window);
    Transform2DSetScale(&cancel_button, 70, 30);
    Transform2DSetPosition(&cancel_button, (size.x - 190) * 2, (size.y - 45) * 2);
    WidgetConnect(&cancel_button, ENGINE_WIDGET_TRIGGER_BUTTON_PRESS, FileExplorerClose, NULL);

    ButtonWidgetInit(&accept_button, "Применить", &exploer_dParam, &explorer_window);
    Transform2DSetScale(&accept_button, 90, 30);
    Transform2DSetPosition(&accept_button, (size.x - 100) * 2, (size.y - 45) * 2);
    WidgetConnect(&accept_button, ENGINE_WIDGET_TRIGGER_BUTTON_PRESS, FileExplorerAccept, NULL);

    ParcePathForStruct(some_dir, &my_path);

    ScrollWidgetUpdate(&files_scroll, &list_files);
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
