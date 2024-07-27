#include "Scene3D.h"
#include <LuaBridge/LuaBridge.h>
#include "Application.h"
#include "raylib_script_extensions.h"
#include "components3D.h"
#include "ModelManager.h"
#include "modules/marble_module.h"

Scene3D::Scene3D(std::string name, std::string path)
	: Scene(name, path)
{
	scene_camera.position = Vector3{ 10.0f, 10.0f, 10.0f }; // Camera position
	scene_camera.target = Vector3{ 0.0f, 0.0f, 0.0f };      // Camera looking at point
	scene_camera.up = Vector3{ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
	scene_camera.fovy = 45.0f;                                // Camera field-of-view Y
	scene_camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type
}

Scene3D::~Scene3D()
{
	script.run("onEnd", this);
	ecs.reset();
}

void Scene3D::initialize()
{
	ModelManager::Initialize("assets/shaders/shadowmap.vs", "assets/shaders/shadowmap.fs", 1028);

	//set ecs modules
	core_module = CoreModule(ecs);
	//setup camera controller

	//setup physics
	physics.initialize(ecs);
	//setup player

	//load script

	//add post processing effects

	this->postProcessor.AddShader("assets/shaders/grayscale.fs");
	this->postProcessor.AddShader("assets/shaders/posterization.fs");
	this->postProcessor.AddShader("assets/shaders/dream_vision.fs");
	this->postProcessor.AddShader("assets/shaders/pixelizer.fs");
	this->postProcessor.AddShader("assets/shaders/cross_hatching.fs");
	this->postProcessor.AddShader("assets/shaders/cross_stitching.fs");
	this->postProcessor.AddShader("assets/shaders/predator.fs");
	this->postProcessor.AddShader("assets/shaders/scanlines.fs");
	this->postProcessor.AddShader("assets/shaders/fisheye.fs");
	this->postProcessor.AddShader("assets/shaders/sobel.fs");
	this->postProcessor.AddShader("assets/shaders/bloom.fs");
	this->postProcessor.AddShader("assets/shaders/blur.fs");
	this->postProcessor.current_shader = -1;
	if (script.Initialize(this->filePath.c_str(), Scene3D::Extend))
	{
		script.run("onStart", this, &this->ecs);
	}
	//ModelManager::lighting.Initialize("assets/shaders/shadowmap.vs", "assets/shaders/shadowmap.fs", 2048);
	EnableCursor();
}

void Scene3D::update(const float& dt)
{
	if(enable_camera_controller)
		UpdateCamera(&scene_camera, camera_mode);
	ecs.progress(dt);
	core_module.update(dt);
}

void Scene3D::fixedUpdate(const float& timestep)
{
	physics.fixedUpdate(timestep);
	core_module.fixedUpdate(timestep);
}

void Scene3D::Render(Color& clearColor, DrawHook onInspect)
{
	BeginDrawing();
	
	if (draw_shadows) {
		ModelManager::lighting.DrawShadowMap(clearColor, std::bind(&Scene3D::DrawWorld, this));
		ModelManager::lighting.Update(scene_camera, 0.0f);
	}

		postProcessor.ApplyEffect();
			ClearBackground(clearColor);
			BeginMode3D(scene_camera);
				DrawWorld();
			EndMode3D();
		postProcessor.EndEffect();

	core_module.drawUI();
	onInspect();
	EndDrawing();
}

void Scene3D::DrawWorld()
{
	if (draw_grid) DrawGrid(grid_slices, grid_spacing);
	core_module.draw3D(scene_camera);
	physics.debugRender();
}

void Scene3D::poll()
{
	core_module.pollEvents();
	physics.poll();
	if (IsKeyReleased(KEY_TAB) && IsKeyDown(KEY_LEFT_SHIFT))
	{
		camera_mode++;
		if (camera_mode > CAMERA_THIRD_PERSON)
		{
			camera_mode = CAMERA_CUSTOM;
		}
	}
	else if  (IsKeyReleased(KEY_TAB) && !IsKeyDown(KEY_LEFT_SHIFT) && enable_camera_controller)
	{
		lock_cursor_to_screen = !lock_cursor_to_screen;
		if (lock_cursor_to_screen)
			DisableCursor();
		else
			EnableCursor();
	}
	if (IsKeyReleased(KEY_GRAVE) )
	{
		postProcessor.current_shader++;

		if (postProcessor.current_shader >= postProcessor.shaders.size())
			postProcessor.current_shader = -1;
	}

}

static const char* cam_modes[] = { "CAMERA_CUSTOM","CAMERA_FREE","CAMERA_ORBITAL","CAMERA_FIRST_PERSON","CAMERA_THIRD_PERSON" };

#include <imgui.h>
void Scene3D::Inspect()
{
	ImGui::Checkbox("draw shadows", &draw_shadows);
	ImGui::SliderInt("camera mode", &camera_mode, CAMERA_CUSTOM, CAMERA_THIRD_PERSON, cam_modes[camera_mode]);

	if (ImGui::TreeNode("Camera3D: main_camera"))
	{
		ImGui::DragFloat3("position", &scene_camera.position.x);
		ImGui::DragFloat3("target", &scene_camera.target.x);
		ImGui::SliderFloat3("up", &scene_camera.up.x, -1, 1);
		ImGui::SliderFloat("fov",&scene_camera.fovy, -360, 360);
		ImGui::SliderInt("projection", &scene_camera.projection, CAMERA_PERSPECTIVE, CAMERA_ORTHOGRAPHIC, scene_camera.projection == 0 ? "PERSPECTIVE" : "ORTHOGRAPHIC");
		ImGui::TreePop();
	}
	ModelManager::lighting.Inspect();
	physics.inspect();
	core_module.InspectComponents();
}

flecs::entity create_entity(flecs::world* world)
{
	flecs::entity e = world->entity();
	return e;
}
flecs::entity create_named_entity(flecs::world* world, const char* name)
{
	flecs::entity e = world->entity(name);
	return e;
}
void Scene3D::Extend(lua_State* L)
{
	raylib::script_extend(L);
	CoreModule::Extend(L);
	App::Extend(L);
	PostProcessor::Extend(L);
	ModelManager::Extend(L);
	//LightingShader::Extend(L);
	core::Physics3D::Extend(L);
	marble::Module::Extend(L);

	luabridge::getGlobalNamespace(L)
		.beginNamespace("flecs")
			.beginClass<flecs::world>("world")
				.addFunction("create_entity", create_entity)
				.addFunction("create_named_entity", create_named_entity)
			.endClass()
			.beginClass<flecs::entity>("entity")
				.addFunction("id", &flecs::entity::id)
				.addFunction("destruct", &flecs::entity::destruct)
				.addFunction("is_alive", &flecs::entity::is_alive)
				.addFunction("is_valid", &flecs::entity::is_valid)
			.endClass()
			.beginClass<flecs::observer>("observer")
			.endClass()
		.endNamespace()
		.beginClass<Scene3D>("Scene3D")
			.addData("scene_camera", &Scene3D::scene_camera)
			.addData("camera_mode", &Scene3D::camera_mode)
			.addData("core_module", &Scene3D::core_module)
			.addData("draw_grid", &Scene3D::draw_grid)
			.addData("grid_slices", &Scene3D::grid_slices)
			.addData("grid_spacing", &Scene3D::grid_spacing)
			.addData("lock_cursor_to_screen", &Scene3D::lock_cursor_to_screen)
			.addData("postProcessor", &Scene3D::postProcessor)
			.addData("draw_shadows", &Scene3D::draw_shadows)
			.addData("enable_camera_controller", &Scene3D::enable_camera_controller)

			.addData("physics", &Scene3D::physics)
		.endClass();
}
static int results = SceneManager::AddScenesFromDirectory("scripts/3D", "3D Scenes", Scene3D::Register);
