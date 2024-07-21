#pragma once
#include <raylib/raylib.h>
#include <lua.hpp>

namespace core
{
	struct Transform3D
	{
		Vector3 position = Vector3{ 0.0f, 0.0f, 0.0f };
		Vector3 scale = Vector3{ 1.0f, 1.0f, 1.0f };
		Vector3 rotationAxis = Vector3{ 0.0f, 1.0f, 0.0f };
		float rotationAngle;
	};

	struct Material
	{
		Color tint = WHITE;
	};
	struct ObjectModel
	{
		unsigned int mId = 0; //Model Manager: Model id;
	};


	void Extend3Dcomponents(lua_State* L);

}

