#pragma once
#include <flecs.h>
#include "components3D.h"
namespace core
{
	flecs::system DrawCubes(flecs::world& ecs);
	flecs::system DrawSpheres(flecs::world& ecs);
	flecs::system DrawModels(flecs::world& ecs);
	flecs::system DrawModelsInstanced(flecs::world& ecs);
}
