#include "resource.h"

EWidgetWindow transform_window;
EWidgetWindow list_window;
EWidgetWindow explorer_window;
EWidgetWindow params_window;
EWidgetWindow console_window;

void **objects;
uint32_t num_objects;

int curr_object = -1;

bool editor_focus = false;

float yaw = 90.f;
float pitch = 0;

char editor_path[] = "/home/ilia/Projects/EngineEditor/";

RenderTexture render_window;
