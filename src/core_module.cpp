#include "core_module.h"
#include "Render3D.h"
#include <LuaBridge/LuaBridge.h>
#include "ModelManager.h"


CoreModule::CoreModule()
{
}

CoreModule::CoreModule(flecs::world& world)
{
	draw3DModels = core::DrawModels(world);

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
	draw3DModels.run();
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

flecs::entity AddCube(flecs::world* world, Vector3 position, Vector3 scale, Color tint)
{
	flecs::entity e = world->entity();
	e.set<core::Transform3D>(core::Transform3D{ position, scale });
	e.set<core::Material>(core::Material{ tint });
	e.set<core::ObjectModel>(core::ObjectModel{ ModelManager::cubeId });

	return e;
}

flecs::entity AddSphere(flecs::world* world, Vector3 position, float radius, Color tint)
{
	flecs::entity e = world->entity();

	e.set<core::Transform3D>(core::Transform3D{ position, Vector3{radius, radius, radius } });
	e.set<core::Material>(core::Material{ tint });
	e.set<core::ObjectModel>(core::ObjectModel{ ModelManager::sphereId });

	return e;
}

flecs::entity AddModel(flecs::world* world, unsigned int modelId, Vector3 position, Vector3 scale, Color tint)
{
	flecs::entity e = world->entity();
	e.set<core::Transform3D>(core::Transform3D{ position, scale });
	e.set<core::Material>(core::Material{ tint });
	e.set<core::ObjectModel>(core::ObjectModel{ modelId });

	return e;
}
/*
*/ 


void CoreModule::Extend(lua_State* L)
{
	core::Extend3Dcomponents(L);
	luabridge::getGlobalNamespace(L)
		.beginNamespace("core")
			.addFunction("AddCube", AddCube)
			.addFunction("AddSphere", AddSphere)
			.addFunction("AddModel", AddModel)
		.endNamespace();
}
