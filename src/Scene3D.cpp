#include "Scene3D.h"
#include <LuaBridge/LuaBridge.h>
#include "Application.h"
#include "raylib_script_extensions.h"
#include "components3D.h"
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
	//set ecs modules
	core_module = CoreModule(ecs);
	//setup camera controller

	//setup physics

	//setup player

	//load script
	if (script.Initialize(this->filePath.c_str(), Scene3D::Extend))
	{
		script.run("onStart", this, &this->ecs);
	}

}

void Scene3D::update(const float& dt)
{
	ecs.progress(dt);
	core_module.update(dt);
}

void Scene3D::fixedUpdate(const float& timestep)
{
	core_module.fixedUpdate(timestep);
}

void Scene3D::draw()
{
	UpdateCamera(&scene_camera, camera_mode);


	BeginMode3D(scene_camera);
		core_module.draw3D(scene_camera);
	EndMode3D();
	core_module.drawUI();
}

void Scene3D::poll()
{
	core_module.pollEvents();
}
#include <imgui.h>
void Scene3D::Inspect()
{
	if (ImGui::TreeNode("Camera3D: main_camera"))
	{
		ImGui::DragFloat3("position", &scene_camera.position.x);
		ImGui::DragFloat3("target", &scene_camera.target.x);
		ImGui::SliderFloat3("up", &scene_camera.up.x, -1, 1);
		ImGui::SliderFloat("fov",&scene_camera.fovy, -360, 360);
		ImGui::SliderInt("projection", &scene_camera.projection, CAMERA_PERSPECTIVE, CAMERA_ORTHOGRAPHIC, scene_camera.projection == 0 ? "PERSPECTIVE" : "ORTHOGRAPHIC");
		ImGui::TreePop();
	}

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
		.endClass();
}
static int results = SceneManager::AddScenesFromDirectory("scripts/3D", "3D Scenes", Scene3D::Register);
