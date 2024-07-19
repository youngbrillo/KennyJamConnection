#pragma once
#include "Scene.h"
#include "Script.h"
#include "flecs.h"

class Scene3D : public Scene
{
public:
	lua::Script script;
	Camera3D scene_camera;
	flecs::world ecs;
public:
	Scene3D(std::string name, std::string path);
	virtual ~Scene3D();
	virtual void initialize();
	virtual void update(const float& dt);
	virtual void fixedUpdate(const float& timestep);
	virtual void draw();
	virtual void poll();
	virtual void Inspect();
};

