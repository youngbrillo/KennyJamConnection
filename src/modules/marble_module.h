#pragma once
#include <flecs.h>
#include <lua.hpp>
#include <raylib/raylib.h>
#include <raylib/raymath.h>

namespace marble
{
	struct Game
	{

	};
	struct Ball {};

	struct Joiner
	{
		Vector3 localA = { 01.0f, 00.0f, 00.0f };
		Vector3 localB = { -1.0f, 00.0f, 00.0f };
		Vector3 localC = { 00.0f, 00.0f, 01.0f };
		Vector3 localD = { 00.0f, 00.0f, -1.0f };
	};

	struct Goal {};

	struct LaunchLocation {
		Vector3 position = { 0.0f, 0.0f, 0.0f };
		Vector3 Velocity = { 1.0f, 0.0f, 0.0f };
	};
	class Module
	{
	public:
		Module(flecs::world& world);

		static void Extend(lua_State* L);
	};
}
