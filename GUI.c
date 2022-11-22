#include "GUI.h"

#include "e_widget_window.h"
#include "e_widget_combobox.h"
#include "e_widget_range.h"

EWidgetWindow window;

EWidgetCombobox object_sellect;
EWidgetButton create_object;

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
EWidgetText label_position_value_x;
EWidgetRange positionX;
float pos_x;
EWidgetText label_position_value_y;
EWidgetRange positionY;
float pos_y;
EWidgetText label_position_value_z;
EWidgetRange positionZ;
float pos_z;

EWidgetText label_size;
EWidgetText label_size_value_x;
EWidgetRange sizeX;
float size_x;
EWidgetText label_size_value_y;
EWidgetRange sizeY;
float size_y;
EWidgetText label_size_value_z;
EWidgetRange sizeZ;
float size_z;

void MakeTransformBlock(EWidgetRange *range, EWidgetText *label, float *value, float x, float y)
{

    RangeWidgetInit( range, 100, 20, 0, 360, &window );
    RangeWidgetSetValueDestin( range, value );
    Transform2DSetPosition( range, x, y );
    TextWidgetInit( label, 9, NULL, &window );
    Transform2DSetPosition( label, x + 220, y + 30 );
}

void GUI_Init()
{
    vec2 size = {400, 300};
    vec2 pos = {0, 0};
    WindowWidgetInit(&window, "Объект", size, NULL, pos);

    vec4 color = { 1, 1, 1, 1 };
    ButtonWidgetInit(&create_object, "Создать", color, &window);
    Transform2DSetScale(&create_object, 100, 30);
    Transform2DSetPosition(&create_object, 250, 0);

    TextWidgetInit(&label_rotation, 9, NULL, &window);
    TextWidgetSetText(&label_rotation, "Поворот");
    Transform2DSetPosition(&label_rotation, 0, 90);

    MakeTransformBlock(&rotationX, &label_rotation_value_x, &rot_x, 0, 100);
    MakeTransformBlock(&rotationY, &label_rotation_value_y, &rot_y, 0, 140);
    MakeTransformBlock(&rotationZ, &label_rotation_value_z, &rot_z, 0, 180);

    TextWidgetInit(&label_position, 9, NULL, &window);
    TextWidgetSetText(&label_position, "Позиция");
    Transform2DSetPosition(&label_position, 0, 250);
    MakeTransformBlock(&positionX, &label_position_value_x, &pos_x, 0, 260);
    MakeTransformBlock(&positionY, &label_position_value_y, &pos_y, 0, 300);
    MakeTransformBlock(&positionZ, &label_position_value_z, &pos_z, 0, 340);

    TextWidgetInit(&label_size, 9, NULL, &window);
    TextWidgetSetText(&label_size, "Размер");
    Transform2DSetPosition(&label_size, 0, 410);
    MakeTransformBlock(&sizeX, &label_size_value_x, &size_x, 0, 420);
    MakeTransformBlock(&sizeY, &label_size_value_y, &size_y, 0, 460);
    MakeTransformBlock(&sizeZ, &label_size_value_z, &size_z, 0, 500);

    ComboboxWidgetInit(&object_sellect, &window);
    ComboboxWidgetAddItem(&object_sellect, "Квадрат");
    ComboboxWidgetAddItem(&object_sellect, "Куб");
    ComboboxWidgetAddItem(&object_sellect, "Сфера");
    ComboboxWidgetAddItem(&object_sellect, "Плоскость");
    ComboboxWidgetAddItem(&object_sellect, "Местность");
    Transform2DSetPosition(&object_sellect, 0, 0);
}

void SetLabelNumberToText(EWidgetText *label, float num)
{
    char buff[6];
    gcvt(num, 4, buff);
    TextWidgetSetText(label, buff);
}

void GUI_Update()
{
    SetLabelNumberToText(&label_rotation_value_x, rot_x);
    SetLabelNumberToText(&label_rotation_value_y, rot_y);
    SetLabelNumberToText(&label_rotation_value_z, rot_z);

    SetLabelNumberToText(&label_position_value_x, pos_x);
    SetLabelNumberToText(&label_position_value_y, pos_y);
    SetLabelNumberToText(&label_position_value_z, pos_z);

    SetLabelNumberToText(&label_size_value_x, size_x);
    SetLabelNumberToText(&label_size_value_y, size_y);
    SetLabelNumberToText(&label_size_value_z, size_z);

    WindowWidgetUpdate(&window);
}

void GUI_Draw()
{
    WindowWidgetDraw(&window);
}

void GUI_Destroy()
{
    WindowWidgetDestroy(&window);
}
