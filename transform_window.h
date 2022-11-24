#ifndef TRANSFORM_WINDOW_H
#define TRANSFORM_WINDOW_H

#include "engine_includes.h"

void TransformWindowInit();
void TransformWindowUpdate(float delta_time);
void TransformWindowDraw();
void TransformWindowDestroy();

void TransformWindowSetValues(vec3 position, vec3 rotation, vec3 scale);

#endif // TRANSFORM_WINDOW_H
