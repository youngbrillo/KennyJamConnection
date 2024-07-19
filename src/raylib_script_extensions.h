#pragma once
#include <raylib/raylib.h>
#include <raylib/raymath.h>
#include <raylib/rlgl.h>

#include <lua.hpp>

namespace raylib
{
	void script_extend(lua_State* L);
}

