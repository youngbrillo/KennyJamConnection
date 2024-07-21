#include "Render3D.h"
#include "ModelManager.h"
void renderModel(const core::Transform3D& transform, const core::ObjectModel& model, const core::Material& mat){
	//DrawModelEx(model.model, transform.position, transform.rotationAxis, transform.rotationAngle, transform.scale, mat.tint);

	Model* m = ModelManager::GetModel(model.mId);
	if (m == NULL)  return;
	DrawModelEx(*m, transform.position, transform.rotationAxis, transform.rotationAngle, transform.scale, mat.tint);
}


flecs::system core::DrawModels(flecs::world& ecs)
{
	return ecs.system<core::Transform3D, core::ObjectModel, core::Material>("Draw Models").kind(0).each(renderModel);
}

flecs::system core::DrawModelsInstanced(flecs::world& ecs)
{
	return flecs::system();
}
