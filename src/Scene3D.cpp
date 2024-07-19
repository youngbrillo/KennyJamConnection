#include "Scene3D.h"

Scene3D::Scene3D(std::string name, std::string path)
	: Scene(name, path)
{
}

Scene3D::~Scene3D()
{
	ecs.reset();
}

void Scene3D::initialize()
{
}

void Scene3D::update(const float& dt)
{
}

void Scene3D::fixedUpdate(const float& timestep)
{
}

void Scene3D::draw()
{
	BeginMode3D(scene_camera);

	EndMode3D();

}

void Scene3D::poll()
{
}

void Scene3D::Inspect()
{
}
