#include "components3D.h"
#include <LuaBridge/LuaBridge.h>

void core::Extend3Dcomponents(lua_State* L)
{
	luabridge::getGlobalNamespace(L)
	.beginNamespace("core")
		.beginClass<core::Transform3D>("Transform3D")
			.addData("position", &core::Transform3D::position)
			.addData("scale", &core::Transform3D::scale)
			.addData("rotationAxis", &core::Transform3D::rotationAxis)
			.addData("rotationAngle", &core::Transform3D::rotationAngle)
		.endClass()
		.beginClass<core::Material>("Material")
			.addData("tint", &core::Material::tint)
		.endClass()
		.beginClass<core::ObjectModel>("ObjectModel")
			.addData("id", &core::ObjectModel::mId)
		.endClass()
	.endNamespace();
}
