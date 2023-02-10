#ifndef CONSOLE_H
#define CONSOLE_H

void ConsoleWindowResize();

void ConsoleInputText(const char* text);

void ConsoleInit();
void ConsoleUpdate(float delta_time);
void ConsoleDraw();
void ConsoleDestroy();

#endif // CONSOLE_H
