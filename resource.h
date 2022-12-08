#ifndef RESOURCE_H
#define RESOURCE_H

#include "engine_includes.h"
#include "e_widget_window.h"

extern EWidgetWindow transform_window;
extern EWidgetWindow list_window;

extern void **objects;
extern uint32_t num_objects;

extern int curr_object;

extern bool editor_focus;

extern char path[];

extern float yaw;
extern float pitch;

#endif // RESOURCE_H
