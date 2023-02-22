#include "params_window.h"

#include <vulkan/vulkan.h>

#include <engine.h>

#include <gameObject.h>
#include <primitiveObject.h>

#include <e_widget_text.h>
#include <e_widget_entry.h>
#include <e_widget_button.h>
#include <e_widget_window.h>
#include <e_widget_combobox.h>

#include "resource.h"

int last_sellected = -1;

EWidgetCombobox topology_comb;
EWidgetEntry entry_texture_path, entry_shader_frag_path, entry_shader_vert_path;

void ParamsWindowResize()
{
    WidgetRecreate(&params_window.top);
}

void ParamsWindowApplyParams(EWidget *widget, void *entry, void *arg)
{
    if(curr_object == -1)
        return;

    EngineDeviceWaitIdle();

    PrimitiveObject *obj = objects[curr_object];

    GameObject3DDestroy(obj);

    DrawParam dParam;
    memset(&dParam, 0, sizeof(DrawParam));

    char *t_path = EntryWidgetGetText(&entry_texture_path);
    char *s_f_path = EntryWidgetGetText(&entry_shader_frag_path);
    char *s_v_path = EntryWidgetGetText(&entry_shader_vert_path);

    memcpy(dParam.diffuse, t_path, strlen(t_path));
    memcpy(dParam.fragShader, s_f_path, strlen(s_f_path));
    memcpy(dParam.vertShader, s_v_path, strlen(s_v_path));

    if(topology_comb.currId == 0)
        dParam.topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
    else
        dParam.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;


    PrimitiveObjectInit(obj, dParam, obj->type, obj->params);
}

void ParamsWindowInit()
{
    vec2 size = { 200, 400};
    vec2 position = {0, 100};
    WindowWidgetInit(&params_window, "Параметры", size, NULL, position);
    params_window.resizeble = false;

    EWidgetText *label_texture_path = calloc(1, sizeof(EWidgetText));
    TextWidgetInit(label_texture_path, 9, NULL, &params_window);
    TextWidgetSetText(label_texture_path, "Diffuse texture :");
    label_texture_path->widget.widget_flags |= ENGINE_FLAG_WIDGET_ALLOCATED;
    Transform2DSetPosition(label_texture_path, 0, 50);

    EntryWidgetInit(&entry_texture_path, 4, &params_window);
    Transform2DSetPosition(&entry_texture_path, 0, 60);
    Transform2DSetScale(&entry_texture_path, 200, 30);

    EWidgetText *label_shader_frag_path = calloc(1, sizeof(EWidgetText));
    TextWidgetInit(label_shader_frag_path, 9, NULL, &params_window);
    TextWidgetSetText(label_shader_frag_path, "Shader frag :");
    label_shader_frag_path->widget.widget_flags |= ENGINE_FLAG_WIDGET_ALLOCATED;
    Transform2DSetPosition(label_shader_frag_path, 0, 150);

    EntryWidgetInit(&entry_shader_frag_path, 4, &params_window);
    Transform2DSetPosition(&entry_shader_frag_path, 0, 160);
    Transform2DSetScale(&entry_shader_frag_path, 200, 30);

    EWidgetText *label_shader_vert_path = calloc(1, sizeof(EWidgetText));
    TextWidgetInit(label_shader_vert_path, 9, NULL, &params_window);
    TextWidgetSetText(label_shader_vert_path, "Shader vert :");
    label_shader_vert_path->widget.widget_flags |= ENGINE_FLAG_WIDGET_ALLOCATED;
    Transform2DSetPosition(label_shader_vert_path, 0, 250);

    EntryWidgetInit(&entry_shader_vert_path, 4, &params_window);
    Transform2DSetPosition(&entry_shader_vert_path, 0, 260);
    Transform2DSetScale(&entry_shader_vert_path, 200, 30);

    size.y = 30;
    ComboboxWidgetInit(&topology_comb, size, &params_window);
    Transform2DSetPosition(&topology_comb, 0, 360);
    Transform2DSetScale(&topology_comb, 200, 30);

    ComboboxWidgetAddItem(&topology_comb, "TOPOLOGY_LINE");
    ComboboxWidgetAddItem(&topology_comb, "TOPOLOGY_TRIANGLE");

    EWidgetButton *accept = calloc(1, sizeof(EWidgetButton));
    ButtonWidgetInit(accept, "Применить", &params_window);
    accept->widget.widget_flags |= ENGINE_FLAG_WIDGET_ALLOCATED;
    Transform2DSetPosition(accept, 0, 550);
    Transform2DSetScale(accept, 60, 30);
    WidgetConnect(accept, ENGINE_WIDGET_TRIGGER_BUTTON_PRESS, (void *)ParamsWindowApplyParams, NULL);
}

void ParamsWindowUpdate()
{
    PrimitiveObject *p_object;

    if(last_sellected != curr_object)
    {
        if(curr_object < 0)
        {
            last_sellected = curr_object;
            return;
        }

        p_object = objects[curr_object];

        EntryWidgetSetText(&entry_texture_path, p_object->go.diffuse->path);

        EntryWidgetSetText(&entry_shader_frag_path, p_object->go.graphObj.aShader.fragShader);
        EntryWidgetSetText(&entry_shader_vert_path, p_object->go.graphObj.aShader.vertShader);

        last_sellected = curr_object;
    }

    WindowWidgetUpdate(&params_window);

}

void ParamsWindowDraw()
{
    WindowWidgetDraw(&params_window);
}

void ParamsWindowDestroy()
{
    WindowWidgetDestroy(&params_window);
}
