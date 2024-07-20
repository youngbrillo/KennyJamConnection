//#include "../Scene.h"
//#include <raylib/raylib.h>
//#include <raylib/raymath.h>
//
//#define RLIGHTS_IMPLEMENTATION
//#include "../rlights.h"
//class Shaders_Basic_Lighting_Scene : public Scene
//{
//public:
//    Camera camera = { 0 };
//    Shader shader;
//    Light lights[4] = { 0 };
//
//public:
//	Shaders_Basic_Lighting_Scene() : Scene("Basic ighting", "") 
//    {
//        // Load basic lighting shader
//        shader = LoadShader(
//            "assets/shaders/lighting.vs",
//            "assets/shaders/lighting.fs");
//
//        // Define the camera to look into our 3d world
//        camera.position = Vector3{ 2.0f, 4.0f, 6.0f };    // Camera position
//        camera.target = Vector3{ 0.0f, 0.5f, 0.0f };      // Camera looking at point
//        camera.up = Vector3{ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
//        camera.fovy = 45.0f;                                // Camera field-of-view Y
//        camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type
//
//    }
//	virtual ~Shaders_Basic_Lighting_Scene() {
//        UnloadShader(shader);
//	}
//	virtual void initialize() {
//       
//
//        // Get some required shader locations
//        shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
//        // NOTE: "matModel" location name is automatically assigned on shader loading, 
//        // no need to get the location again if using that uniform name
//        //shader.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocation(shader, "matModel");
//
//        // Ambient light level (some basic lighting)
//        int ambientLoc = GetShaderLocation(shader, "ambient");
//        float v[4] = {0.1f, 0.1f, 0.1f, 1.0f}; 
//        SetShaderValue(shader, ambientLoc, &v, SHADER_UNIFORM_VEC4);
//
//        // Create lights
//        lights[0] = CreateLight(LIGHT_POINT, Vector3{ -2, 1, -2 }, Vector3Zero(), YELLOW, shader);
//        lights[1] = CreateLight(LIGHT_POINT, Vector3{ 2, 1, 2 }, Vector3Zero(), RED, shader);
//        lights[2] = CreateLight(LIGHT_POINT, Vector3{ -2, 1, 2 }, Vector3Zero(), GREEN, shader);
//        lights[3] = CreateLight(LIGHT_POINT, Vector3{ 2, 1, -2 }, Vector3Zero(), BLUE, shader);
//
//	}
//    virtual void update(const float& dt)
//    {
//
//        // Update
//        //----------------------------------------------------------------------------------
//        UpdateCamera(&camera, CAMERA_ORBITAL);
//
//        // Update the shader with the camera view vector (points towards { 0.0f, 0.0f, 0.0f })
//        float cameraPos[3] = { camera.position.x, camera.position.y, camera.position.z };
//        SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);
//
//
//    }
//    virtual void fixedUpdate(const float& timestep)
//    {
//
//    }
//    virtual void predraw() 
//    {
//
//    }
//    virtual void draw()
//    {
//        BeginMode3D(camera);
//            BeginShaderMode(shader);
//                DrawPlane(Vector3Zero(), Vector2 { 10.0, 10.0 }, WHITE);
//                DrawCube(Vector3Zero(), 2.0, 4.0, 2.0, WHITE);
//            EndShaderMode();
//            // Draw spheres to show where the lights are
//            for (int i = 0; i < 4; i++)
//            {
//                if (lights[i].enabled) DrawSphereEx(lights[i].position, 0.2f, 8, 8, lights[i].color);
//                else DrawSphereWires(lights[i].position, 0.2f, 8, 8, ColorAlpha(lights[i].color, 0.3f));
//            }
//
//            DrawGrid(10, 1.0f);
//
//        EndMode3D();
//
//        DrawText("Use keys [Y][R][G][B] to toggle lights", 10, 40, 20, DARKGRAY);
//
//    }
//    virtual void poll()
//    {
//        // Check key inputs to enable/disable lights
//        if (IsKeyPressed(KEY_Y)) { lights[0].enabled = !lights[0].enabled; }
//        if (IsKeyPressed(KEY_R)) { lights[1].enabled = !lights[1].enabled; }
//        if (IsKeyPressed(KEY_G)) { lights[2].enabled = !lights[2].enabled; }
//        if (IsKeyPressed(KEY_B)) { lights[3].enabled = !lights[3].enabled; }
//    }
//    virtual void Inspect()
//    {
//
//    }
//};
//
//Shaders_Basic_Lighting_Scene* create_new_scene(std::string, std::string)
//{
//    return new Shaders_Basic_Lighting_Scene();
//}
//
//static int entry = SceneEntry::Register("Shaders", "Basic Lighting", "", create_new_scene);