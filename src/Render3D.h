#pragma once
#include <flecs.h>
#include "components3D.h"
namespace core
{
	flecs::system DrawModels(flecs::world& ecs);
	flecs::system DrawModelsInstanced(flecs::world& ecs);
}
