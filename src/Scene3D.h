#pragma once
#include "Scene.h"
#include "Script.h"
#include "flecs.h"
#include "core_module.h"
class Scene3D : public Scene
{
public:
	lua::Script script;
	Camera3D scene_camera;
	flecs::world ecs;
	CoreModule core_module;
public:
	Scene3D(std::string name, std::string path);
	virtual ~Scene3D();
	virtual void initialize();
	virtual void update(const float& dt);
	virtual void fixedUpdate(const float& timestep);
	virtual void draw();
	virtual void poll();
	virtual void Inspect();

public:
	static void Extend(lua_State* L);
	static Scene3D* Register(std::string name, std::string path){return new Scene3D(name, path);}
};

