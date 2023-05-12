#include "editor.h"

#include <engine.h>
#include <render_texture.h>
#include <gameObject.h>
#include <gameObject3D.h>
#include <models.h>
#include <glTFLoader.h>

#include <e_camera.h>

#include <e_math.h>

#include <e_resource_data.h>

#include "GUI.h"


#include "resource.h"

Camera3D camera;

ModelObject3D axis_model;


float lastX = 0, lastY = 0;
float startX, startY;

float moveSpeed = 2.0f;

float cameraSpeed = 1.5f;
float sensitivity = 10.0f;

void CamRotateView(float deltaTime){

    if(!editor_focus)
        return;

    double xpos, ypos;

    EngineGetCursorPos(&xpos, &ypos);
    EngineFixedCursorCenter();

    float xoffset = WIDTH/2 - xpos;
    float yoffset = HEIGHT/2 - ypos;

    xoffset *= sensitivity * deltaTime;
    yoffset *= sensitivity * deltaTime;

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    vec3 direction; //= getViewRotation();
    direction.x = cos(yaw * (M_PI / 180)) * cos(pitch * (M_PI / 180));
    direction.y = -sin(pitch * (M_PI / 180));
    direction.z = sin(yaw * (M_PI / 180)) * cos(pitch * (M_PI / 180));
    Camera3DSetRotation(direction.x, direction.y, direction.z);
}

void KeyUpdateInput(float deltaTime)
{
    if(!editor_focus)
        return;

    vec3 up = {0.0f,1.0f,0.0f};

    vec3 pos = Camera3DGetPosition();
    vec3 viewRot = Camera3DGetRotation();

    vec3 forward;
    if (EngineGetKeyPress(ENGINE_KEY_W))
    {
        forward = v3_sub(pos, v3_muls( viewRot, cameraSpeed * deltaTime));
        Camera3DSetPosition(forward.x, forward.y, forward.z);
    }else if (EngineGetKeyPress(ENGINE_KEY_S)){
        forward = v3_add(pos, v3_muls( viewRot, cameraSpeed * deltaTime));
        Camera3DSetPosition(forward.x, forward.y, forward.z);
    }

    if (EngineGetKeyPress(ENGINE_KEY_A)){
        forward = v3_sub(pos, v3_muls(v3_norm(v3_cross(viewRot, up)), cameraSpeed * deltaTime));
        Camera3DSetPosition(forward.x, forward.y, forward.z);
    }else if (EngineGetKeyPress(ENGINE_KEY_D)){
        forward = v3_add(pos, v3_muls(v3_norm(v3_cross(viewRot, up)), cameraSpeed * deltaTime));
        Camera3DSetPosition(forward.x, forward.y, forward.z);
    }

    if (EngineGetKeyPress(ENGINE_KEY_LEFT_SHIFT)){
        cameraSpeed = moveSpeed * 10;
    }else{
        cameraSpeed = moveSpeed * 3;
    }

    if(EngineGetKeyPress(ENGINE_KEY_ESCAPE))
    {
        editor_focus = false;
        EngineHideCursor(2);
    }
}

void Resize()
{
    GUI_Resize();
}

void Init()
{
    Camera3DInit(&camera);

    RenderTextureInit(&render_window, ENGINE_RENDER_TYPE_WINDOW, 0, 0, 0);

    DrawParam dParam;
    memset(&dParam, 0, sizeof(DrawParam));
    dParam.render = &render_window;

    char filePath[256];
    ToolsAddStrings(filePath, 256, editor_path, "models/");
    ToolsAddStrings(dParam.diffuse, 256, editor_path, "textures/texture.jpg");
    Load3DglTFModel(&axis_model, filePath, "Axis_Model", 2, &dParam);
    Transform3DSetRotateT(&axis_model.transform, 0, 0, -90);
    Transform3DSetScaleT(&axis_model.transform, 0.002f, 0.002f, 0.002f);
    Transform3DSetPositionT(&axis_model.transform, -0.7f, -0.5f, 1.5f);
    ModelSetSelCameraEnable(&axis_model, true);
    ModelSetLightEnable(&axis_model, false);


    EngineSetRecreateFunc((void *)Resize);

    GUI_Init();

    vec3 direction; //= getViewRotation();
    direction.x = cos(yaw * (M_PI / 180)) * cos(pitch * (M_PI / 180));
    direction.y = -sin(pitch * (M_PI / 180));
    direction.z = sin(yaw * (M_PI / 180)) * cos(pitch * (M_PI / 180));
    Camera3DSetRotation(direction.x, direction.y, direction.z);
}

void Update(float delta_time)
{
    GUI_Update(delta_time);

    CamRotateView(delta_time);
    KeyUpdateInput(delta_time);

    vec3 up = {0.0f,1.0f,0.0f};
    vec3 cam_dir = v3_muls(v3_norm(Camera3DGetRotation()), -180);
    Transform3DSetRotateT(&axis_model.transform, 0, yaw - 150, pitch);
}

void Draw()
{
    for(int i=0;i< num_objects;i++)
    {
        GameObject3D *obj = objects[i];

        /*if(i == curr_object)
            obj->wired = true;
        else
            obj->wired = false;*/


        EngineDraw(objects[i]);
    }

    GUI_Draw();

    EngineDraw(&axis_model);

    EngineSetRender( &render_window, 1);
}

void Clean()
{
    for(int i=0;i< num_objects;i++)
    {
        GameObjectDestroy(objects[i]);
        free(objects[i]);
    }
    free(objects);

    GameObjectDestroy(&axis_model);

    GUI_Destroy();

    RenderTextureDestroy(&render_window);
}
