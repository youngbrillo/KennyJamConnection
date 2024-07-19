#pragma once
#include <flecs.h>
#include <raylib/raylib.h>
#include <lua.hpp>

class CoreModule
{
public:
	flecs::system draw2Dsystem;
	flecs::system draw3Dsystem;
	flecs::system drawUIsystem;
	flecs::system pollSystem;
	flecs::system inspectSystem;
public:
	CoreModule();
	CoreModule(flecs::world& world);

	void update(const float& dt);
	void fixedUpdate(const float& timestep);
	void draw2D(Camera2D& camera);
	void draw3D(Camera3D& camera);
	void drawUI();
	void pollEvents();
	void InspectComponents();

	static void Extend(lua_State* L);
};

