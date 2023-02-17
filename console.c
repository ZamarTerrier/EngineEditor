#include "console.h"

#include <e_widget_entry_area.h>

#include "resource.h"

EWidgetEntryArea area;

void ConsoleWindowResize()
{
    WidgetRecreate(&console_window.top);
}

void ConsoleInputText(const char* text)
{
    EntryAreaWidgetInsertText(&area, text);
}

void ConsoleInit()
{
    vec2 size = { 400, 200};
    vec2 position = {0, 1100};
    WindowWidgetInit(&console_window, "Консоль", size, NULL, position);
    console_window.resizeble = false;

    EntryAreaWidgetInit(&area, 9, &console_window);
    Transform2DSetScale(&area, size.x, size.y - 10);
    area.entry.height = size.y - 10;
    area.entry.width = size.x * 1.5f;

    area.entry.widget.widget_flags = ENGINE_FLAG_WIDGET_ACTIVE;
}

void ConsoleUpdate(float delta_time)
{
    WindowWidgetUpdate(&console_window);
}

void ConsoleDraw()
{
    WindowWidgetDraw(&console_window);
}

void ConsoleDestroy()
{
    WindowWidgetDestroy(&console_window);
}
