#include "core_module.h"

CoreModule::CoreModule()
{
}

CoreModule::CoreModule(flecs::world& world)
{
}

void CoreModule::update(const float& dt)
{
}

void CoreModule::fixedUpdate(const float& timestep)
{
}

void CoreModule::draw2D(Camera2D& camera)
{
}

void CoreModule::draw3D(Camera3D& camera)
{
}

void CoreModule::drawUI()
{
}

void CoreModule::pollEvents()
{
}

void CoreModule::InspectComponents()
{
}
#include <LuaBridge/LuaBridge.h>
void CoreModule::Extend(lua_State* L)
{
}
