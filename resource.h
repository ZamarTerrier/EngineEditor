#ifndef RESOURCE_H
#define RESOURCE_H

#include "engine_includes.h"
#include "e_widget_window.h"

#include <render_texture.h>

extern EWidgetWindow transform_window;
extern EWidgetWindow list_window;
extern EWidgetWindow explorer_window;
extern EWidgetWindow params_window;
extern EWidgetWindow console_window;

extern void **objects;
extern uint32_t num_objects;

extern int curr_object;

extern bool editor_focus;

extern char editor_path[];

extern float yaw;
extern float pitch;

extern RenderTexture render_window;

#endif // RESOURCE_H
