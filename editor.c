#include "editor.h"

#include "engine.h"
#include "gameObject.h"
#include "gameObject3D.h"
#include "models.h"
#include "glTFLoader.h"

#include "GUI.h"

#include "camera.h"

#include "resource.h"

#include "e_math.h"

Camera3D camera;

ModelObject3D axis_model;

float yaw = 90.f;
float pitch = 0;

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
    setViewRotation(direction.x, direction.y, direction.z);
}

void KeyUpdateInput(float deltaTime)
{
    if(!editor_focus)
        return;

    vec3 up = {0.0f,1.0f,0.0f};

    vec3 pos = getViewPos();
    vec3 viewRot = getViewRotation();

    vec3 forward;
    if (EngineGetKeyPress(ENGINE_KEY_W))
    {
        forward = v3_sub(pos, v3_muls( viewRot, cameraSpeed * deltaTime));
        setViewPos(forward.x, forward.y, forward.z);
    }else if (EngineGetKeyPress(ENGINE_KEY_S)){
        forward = v3_add(pos, v3_muls( viewRot, cameraSpeed * deltaTime));
        setViewPos(forward.x, forward.y, forward.z);
    }

    if (EngineGetKeyPress(ENGINE_KEY_A)){
        forward = v3_sub(pos, v3_muls(v3_norm(v3_cross(viewRot, up)), cameraSpeed * deltaTime));
        setViewPos(forward.x, forward.y, forward.z);
    }else if (EngineGetKeyPress(ENGINE_KEY_D)){
        forward = v3_add(pos, v3_muls(v3_norm(v3_cross(viewRot, up)), cameraSpeed * deltaTime));
        setViewPos(forward.x, forward.y, forward.z);
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

    DrawParam dParam;
    memset(&dParam, 0, sizeof(DrawParam));
    char filePath[256];
    ToolsAddStrings(filePath, 256, path, "models/");
    ToolsAddStrings(dParam.diffuse, 256, path, "textures/texture.jpg");
    Load3DglTFModel(&axis_model, filePath, "Axis_Model", 2, &dParam);
    Transform3DSetRotateT(&axis_model.transform, 0, 0, -90);
    Transform3DSetScaleT(&axis_model.transform, -0.002f, 0.002f, -0.002f);

    EngineSetRecreateFunc((void *)Resize);

    GUI_Init();

}

void Update(float delta_time)
{
    GUI_Update(delta_time);

    CamRotateView(delta_time);
    KeyUpdateInput(delta_time);

    vec3 cam_pos = getViewPos();
    vec3 cam_dir = v3_norm(getViewRotation());
    vec3 m_axis = {0, cam_dir.y, 5};
    vec3 result_pos = v3_add(cam_pos, v3_muls(cam_dir, -1));
    Transform3DSetPositionT(&axis_model.transform, result_pos.x, result_pos.y, result_pos.z);
}

void Draw()
{
    for(int i=0;i< num_objects;i++)
    {
        GameObject3D *obj = objects[i];

        if(i == curr_object)
            obj->wired = true;
        else
            obj->wired = false;


        EngineDraw(objects[i]);
    }

    EngineDraw(&axis_model);

    GUI_Draw();
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
}
