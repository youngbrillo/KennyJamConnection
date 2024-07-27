#pragma once
#include "Scene.h"
#include "Script.h"
#include "flecs.h"
#include "core_module.h"
#include "post_processor.h"
#include "modules/physics3d_module.h"

class Scene3D : public Scene
{
public:
	lua::Script script;
	Camera3D scene_camera;
	flecs::world ecs;
	CoreModule core_module;
	int camera_mode = CAMERA_CUSTOM; 
	bool enable_camera_controller = true;
	bool draw_grid = false;
	int grid_slices = 10, grid_spacing = 1;
	bool lock_cursor_to_screen = false;
	PostProcessor postProcessor;
	bool draw_shadows = true;
	core::Physics3D physics;
public:
	Scene3D(std::string name, std::string path);
	virtual ~Scene3D();
	virtual void initialize();
	virtual void update(const float& dt);
	virtual void fixedUpdate(const float& timestep);
	virtual void Render(Color& clearColor, DrawHook onInspect) override;
	virtual void DrawWorld();
	virtual void poll();
	virtual void Inspect();

public:
	static void Extend(lua_State* L);
	static Scene3D* Register(std::string name, std::string path){return new Scene3D(name, path);}
};

