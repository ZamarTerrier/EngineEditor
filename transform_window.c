
#include <e_widget_window.h>
#include <e_widget_combobox.h>
#include <e_widget_roller.h>
#include <e_widget_range.h>
#include <e_widget_entry.h>

#include <camera.h>

#include <e_math.h>

#include <e_resource_data.h>

#include "resource.h"

EWidgetText label_rotation;
EWidgetText label_rotation_value_x;
float rot_x;
EWidgetRange rotationX;
EWidgetText label_rotation_value_y;
EWidgetRange rotationY;
float rot_y;
EWidgetText label_rotation_value_z;
EWidgetRange rotationZ;
float rot_z;

EWidgetText label_position;
EWidgetText label_position_value_x_label;
EWidgetRoller roller_position_x;
EWidgetEntry entry_position_value_x;
float pos_x;
EWidgetText label_position_value_y_label;
EWidgetRoller roller_position_y;
EWidgetEntry entry_position_value_y;
float pos_y;
EWidgetText label_position_value_z_label;
EWidgetRoller roller_position_z;
EWidgetEntry entry_position_value_z;
float pos_z;

EWidgetText label_size;
EWidgetText label_size_value_x_label;
EWidgetRoller roller_size_x;
EWidgetEntry entry_size_value_x;
float size_x;
EWidgetText label_size_value_y_label;
EWidgetRoller roller_size_y;
EWidgetEntry entry_size_value_y;
float size_y;
EWidgetText label_size_value_z_label;
EWidgetRoller roller_size_z;
EWidgetEntry entry_size_value_z;
float size_z;

EWidgetButton move_to_object;

void TransformRollerMove(EWidget *widget, float *val, EWidgetEntry *entry)
{
    char buff[20];
    sprintf(buff, "%.02f", *val);
    EntryWidgetSetText(entry, buff);
}

void MakeTransformBlock(EWidgetRange *range, EWidgetText *label, float *value, float x, float y)
{
    RangeWidgetInit( range, 100, 20, 0, 360, &transform_window );
    RangeWidgetSetValueDestin( range, value );
    Transform2DSetPosition( range, x, y );
    TextWidgetInit( label, 9, NULL, &transform_window );
    Transform2DSetPosition( label, x + 220, y + 30 );
}

void MakeEntryBlock(EWidgetEntry *entry, EWidgetText *label, EWidgetRoller *roller, float *source, char *text_label, float x, float y)
{
    EntryWidgetInit(entry, 9, &transform_window);
    Transform2DSetScale(entry, 60, 20);
    Transform2DSetPosition(entry, x, y);

    RollerWidgetInit(roller, &transform_window);
    RollerWidgetSetSource(roller, source);
    Transform2DSetPosition(roller, x + 100, y);
    Transform2DSetScale(roller, 20, 20);
    WidgetConnect(roller, ENGINE_WIDGET_TRIGGER_ROLLER_MOVE, TransformRollerMove, entry);

    TextWidgetInit( label, 9, NULL, &transform_window );
    Transform2DSetPosition( label, x - 40, y + 30);
    TextWidgetSetText( label, text_label);
}

void MoveToTarget(EWidget *widget, void *entry, void *args)
{
    if(curr_object == -1)
        return;

    vec3 target = Transform3DGetPosition(objects[curr_object]);
    target.x -=10;
    target.y +=10;
    target.z -=10;

    setViewPos(target.x, target.y, target.z);
    vec3 dir = v3_sub(getViewPos(), Transform3DGetPosition(objects[curr_object]));
    dir = v3_norm(dir);
    setViewRotation(dir.x * 180, dir.y * 180, dir.z * 180);
    yaw = (dir.y * 180) + 120;
    pitch = (dir.z * 180) + 70;
}

void TransformWindowResize()
{
    Transform2DSetPosition(&transform_window.top, (WIDTH * 2) - 260, 60);
    WidgetRecreate(&transform_window.top);
}

void TransformWindowInit()
{
    vec2 size = {130, 450};
    vec2 pos = {(WIDTH * 2) - 260, 60};
    WindowWidgetInit(&transform_window, "Объект", size, NULL, pos);
    transform_window.resizeble = false;

    ButtonWidgetInit(&move_to_object, "Переместиться", &transform_window);
    Transform2DSetScale(&move_to_object, 130, 30);
    Transform2DSetPosition(&move_to_object, 0, 0);
    WidgetConnect(&move_to_object, ENGINE_WIDGET_TRIGGER_BUTTON_PRESS, MoveToTarget, NULL);

    TextWidgetInit(&label_rotation, 9, NULL, &transform_window);
    TextWidgetSetText(&label_rotation, "Поворот");
    Transform2DSetPosition(&label_rotation, 0, 90);
    MakeTransformBlock(&rotationX, &label_rotation_value_x, &rot_x, 0, 100);
    MakeTransformBlock(&rotationY, &label_rotation_value_y, &rot_y, 0, 140);
    MakeTransformBlock(&rotationZ, &label_rotation_value_z, &rot_z, 0, 180);

    TextWidgetInit(&label_position, 9, NULL, &transform_window);
    TextWidgetSetText(&label_position, "Позиция");
    Transform2DSetPosition(&label_position, 0, 270);
    MakeEntryBlock(&entry_position_value_x, &label_position_value_x_label, &roller_position_x, &pos_x, "X :", 50, 290);
    MakeEntryBlock(&entry_position_value_y, &label_position_value_y_label, &roller_position_y, &pos_y, "Y :", 50, 350);
    MakeEntryBlock(&entry_position_value_z, &label_position_value_z_label, &roller_position_z, &pos_z, "Z :", 50, 410);

    TextWidgetInit(&label_size, 9, NULL, &transform_window);
    TextWidgetSetText(&label_size, "Размер");
    Transform2DSetPosition(&label_size, 0, 500);
    MakeEntryBlock(&entry_size_value_x, &label_size_value_x_label, &roller_size_x, &size_x, "X :", 50, 520);
    MakeEntryBlock(&entry_size_value_y, &label_size_value_y_label, &roller_size_y, &size_y, "Y :", 50, 580);
    MakeEntryBlock(&entry_size_value_z, &label_size_value_z_label, &roller_size_z, &size_z, "Z :", 50, 640);
}

void SetLabelNumberToText(EWidgetText *label, float num)
{
    char buff[6];
    gcvt(num, 4, buff);
    TextWidgetSetText(label, buff);
}

void TransformWindowUpdate(float delta_time)
{

    SetLabelNumberToText(&label_rotation_value_x, rot_x);
    SetLabelNumberToText(&label_rotation_value_y, rot_y);
    SetLabelNumberToText(&label_rotation_value_z, rot_z);

    pos_x = atof(EntryWidgetGetText(&entry_position_value_x));
    pos_y = atof(EntryWidgetGetText(&entry_position_value_y));
    pos_z = atof(EntryWidgetGetText(&entry_position_value_z));

    size_x = atof(EntryWidgetGetText(&entry_size_value_x));
    size_y = atof(EntryWidgetGetText(&entry_size_value_y));
    size_z = atof(EntryWidgetGetText(&entry_size_value_z));

    if(curr_object != -1)
    {
        Transform3DSetRotate(objects[curr_object], rot_x, rot_y, rot_z);
        Transform3DSetPosition(objects[curr_object], pos_x, pos_y, pos_z);
        Transform3DSetScale(objects[curr_object], size_x, size_y, size_z);
    }

    WindowWidgetUpdate(&transform_window);
}

void TransformWindowDraw()
{
    WindowWidgetDraw(&transform_window);
}

void TransformWindowDestroy()
{
    WindowWidgetDestroy(&transform_window);
}

void TransformWindowSetValues(vec3 position, vec3 rotation, vec3 scale)
{
    RangeWidgetSetValue(&rotationX, rotation.x);
    rot_x = rotation.x;
    RangeWidgetSetValue(&rotationY, rotation.y);
    rot_y = rotation.y;
    RangeWidgetSetValue(&rotationZ, rotation.z);
    rot_z = rotation.z;

    char buff[20];
    sprintf(buff, "%.02f", position.x);
    EntryWidgetSetText(&entry_position_value_x, buff);
    sprintf(buff, "%.02f", position.y);
    EntryWidgetSetText(&entry_position_value_y, buff);
    sprintf(buff, "%.02f", position.z);
    EntryWidgetSetText(&entry_position_value_z, buff);

    sprintf(buff, "%.02f", scale.x);
    EntryWidgetSetText(&entry_size_value_x, buff);
    sprintf(buff, "%.02f", scale.y);
    EntryWidgetSetText(&entry_size_value_y, buff);
    sprintf(buff, "%.02f", scale.z);
    EntryWidgetSetText(&entry_size_value_z, buff);
}
