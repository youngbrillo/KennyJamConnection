#pragma once
#include <flecs.h>
#include <lua.hpp>
namespace core
{
	struct RigidBody3D
	{
		unsigned int jid = 0;
	};

	struct SoftBody3D
	{
		unsigned int jid = 0;
	};

	class JoltModule
	{
	public:
		JoltModule(flecs::world& world);

		static void Extend(lua_State* L);
	};
}

