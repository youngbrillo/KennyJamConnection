#pragma once
#include "../Scene.h"
#include "../post_processor.h"
#include "../util/LightingSystem.h"
//#include "../post_processor.h"
struct RenderObject3D
{
	int mid = 0;
	Vector3 position = Vector3Zero();
	Vector3 rotationAxis = Vector3{ 0.0f, 1.0f, 0.0f };
	float rotationAngle = 0.0f;
	Vector3 scale = { 1.0f, 1.0f, 1.0f };
	Color tint = WHITE;
};

class ThreeDeeScene : public Scene
{
public:
	Camera3D sceneCam;
	LightingSystem lighting;
	PostProcessor  postProcessor;
	std::vector<Model> model_list;
	std::vector<RenderObject3D> objects;
	int camera_mode = CAMERA_ORBITAL;
public:
	ThreeDeeScene(std::string n, std::string p);
	~ThreeDeeScene();
	//setup
	void resetSceneCamera();
	virtual void initialize();
	virtual void onStart() {}
	virtual void onEnd() {};
	//update
	virtual void update(const float& dt);
	virtual void fixedUpdate(const float& timestep);

	//drawing
	virtual void Render(Color& clearColor, DrawHook onInspect) override;
	virtual void DrawWorld();
	virtual void DrawUI();
	virtual void Inspect();
	//event handling
	virtual void poll();

	//helpers
	int AddModel(Model model);

	//instancing
	static ThreeDeeScene* Create(std::string n, std::string p) { return new ThreeDeeScene(n, p); }

};

