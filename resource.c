#include "resource.h"

EWidgetWindow transform_window;
EWidgetWindow list_window;

void **objects;
uint32_t num_objects;

int curr_object = -1;

bool editor_focus = false;

char path[] = "/home/ilia/Projects/EngineEditor/";
