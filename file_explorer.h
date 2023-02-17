#ifndef FILE_EXPLORER_H
#define FILE_EXPLORER_H

typedef void (*callback_name)(const char *);

void FileExplorertWindowResize();

void FileExplorerWindowInit();
void FileExplorerWindowUpdate(float delta_time);
void FileExplorerWindowDraw();
void FileExplorerWindowDestroy();

void FileExplorerSetCallback(callback_name some_funk);

#endif // FILE_EXPLORER_H
