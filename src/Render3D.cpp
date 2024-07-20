#include "Render3D.h"

void renderCube(const core::Transform3D& transform, core::Cube, const core::Material& mat)
{
	DrawCubeV(transform.position, transform.scale, mat.tint);
}

void renderSphere(const core::Transform3D& transform, const core::Sphere& sphere, const core::Material& mat) {
	DrawSphereEx(transform.position, transform.scale.x, sphere.rings, sphere.slices, mat.tint);
}
void renderModel(const core::Transform3D& transform, const core::ObjectModel& model, const core::Material& mat){
	DrawModelEx(model.model, transform.position, transform.rotationAxis, transform.rotationAngle, transform.scale, mat.tint);
}


flecs::system core::DrawCubes(flecs::world& ecs)
{
	return ecs.system<core::Transform3D, core::Cube, core::Material>("Draw Cubes")
		.kind(0)
		.each(renderCube);
}

flecs::system core::DrawSpheres(flecs::world& ecs)
{
	return ecs.system<core::Transform3D, core::Sphere, core::Material>("Draw Spheres").kind(0).each(renderSphere);
}

flecs::system core::DrawModels(flecs::world& ecs)
{
	return ecs.system<core::Transform3D, core::ObjectModel, core::Material>("Draw Models").kind(0).each(renderModel);
}

flecs::system core::DrawModelsInstanced(flecs::world& ecs)
{
	return flecs::system();
}
