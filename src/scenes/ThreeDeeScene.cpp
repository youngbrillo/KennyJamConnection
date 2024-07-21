#include "ThreeDeeScene.h"
#include <imgui.h>

bool EditColor(Color& color, const char* title = "Color")
{
	Vector4 wc = ColorNormalize(color);
	if (ImGui::ColorEdit4("window color", &wc.x))
	{
		color = ColorFromNormalized(wc);
		return true;
	}
	return false;
}

ThreeDeeScene::ThreeDeeScene(std::string n, std::string p)
	: Scene(n, p)
{
}

ThreeDeeScene::~ThreeDeeScene()
{
	for (auto& m : model_list)
	{
		UnloadModel(m);
	}
}

void ThreeDeeScene::resetSceneCamera()
{
	sceneCam.position = Vector3{ 10.0f, 10.0f, 10.0f };
	sceneCam.target = Vector3Zero();
	sceneCam.projection = CAMERA_PERSPECTIVE;
	sceneCam.up = Vector3{ 0.0f, 1.0f, 0.0f };
	sceneCam.fovy = 45.0f;
}

void ThreeDeeScene::initialize()
{
	resetSceneCamera();
	lighting.Initialize("assets/shaders/shadowmap.vs", "assets/shaders/shadowmap.fs", 2048);

	//load models
	int cube = AddModel(LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f)));
	int robot = AddModel(LoadModel("resources/models/robot.glb"));

	//add objects
	RenderObject3D obj;
	//floor
	obj = RenderObject3D{ cube, Vector3Zero(), Vector3{ 0.0f, 1.0f, 0.0f }, 0.0f, Vector3{ 20.0f, 1.0f, 20.0f }, BLUE };
	objects.push_back(obj);
	//regular cube
	obj = RenderObject3D{ cube,Vector3{ 1.5f, 1.0f, -1.5f }, Vector3{ 0.0f, 1.0f, 0.0f }, 0.0f, Vector3One(), WHITE };
	objects.push_back(obj);
	//robot
	obj = RenderObject3D{ robot, Vector3{ 0.0f, 0.5f, 0.0f }, Vector3{ 0.0f, 1.0f, 0.0f }, 0.0f, Vector3{ 1.0f, 1.0f, 1.0f }, RED };
	objects.push_back(obj);


	//add post processing effects

	this->postProcessor.AddShader("assets/shaders/grayscale.fs");
	this->postProcessor.AddShader("assets/shaders/posterization.fs");
	this->postProcessor.AddShader("assets/shaders/dream_vision.fs");
	this->postProcessor.AddShader("assets/shaders/pixelizer.fs");
	this->postProcessor.AddShader("assets/shaders/cross_hatching.fs");
	this->postProcessor.AddShader("assets/shaders/cross_stitching.fs");
	this->postProcessor.AddShader("assets/shaders/predator.fs");
	this->postProcessor.AddShader("assets/shaders/scanlines.fs");
	this->postProcessor.AddShader("assets/shaders/fisheye.fs");
	this->postProcessor.AddShader("assets/shaders/sobel.fs");
	this->postProcessor.AddShader("assets/shaders/bloom.fs");
	this->postProcessor.AddShader("assets/shaders/blur.fs");

	postProcessor.current_shader = -1;
}

void ThreeDeeScene::update(const float& dt)
{
	UpdateCamera(&sceneCam, camera_mode);
	lighting.Update(sceneCam, dt);
}

void ThreeDeeScene::fixedUpdate(const float& timestep)
{
} 

void ThreeDeeScene::Render(Color& clearColor, DrawHook onInspect)
{
	BeginDrawing();
	//ClearBackground(clearColor);
	lighting.DrawShadowMap(clearColor, std::bind(&ThreeDeeScene::DrawWorld, this));

	//post proccessor here
	postProcessor.ApplyEffect();
		ClearBackground(clearColor);

		BeginMode3D(sceneCam);
			DrawWorld();
		EndMode3D();
	postProcessor.EndEffect();
	DrawUI();
	onInspect();
	EndDrawing();
}

void ThreeDeeScene::DrawWorld()
{
	for (const RenderObject3D& o : objects)
	{
		DrawModelEx(model_list[o.mid], o.position, o.rotationAxis, o.rotationAngle, o.scale, o.tint);
	}
}

static const char* CameraModes(int mode)
{
	switch (mode)
	{
	case CAMERA_CUSTOM: return "CUSTOM CAMERA";
	case CAMERA_FREE: return "FREE CAMERA";
	case CAMERA_ORBITAL: return  "ORBITAL CAMERA";
	case CAMERA_FIRST_PERSON: return "FIRST PERSON CAMERA";
	case CAMERA_THIRD_PERSON: return "THIRD PERSON CAMERA";
	default:
		return "UNKNOWN";
	}
}

void ThreeDeeScene::DrawUI()
{
	DrawText(name.c_str(), 25, GetScreenHeight() - 34, 24, GRAY);
	DrawText(CameraModes(camera_mode), 25, GetScreenHeight() - 64, 24, WHITE);
}

void ThreeDeeScene::Inspect()
{
	lighting.Inspect();
	if (IsKeyReleased(KEY_TAB))
	{
		camera_mode++;
		if (camera_mode > CAMERA_THIRD_PERSON)
		{
			camera_mode = CAMERA_CUSTOM;
		}
	}
	if (IsKeyReleased(KEY_GRAVE))
	{
		postProcessor.current_shader++;

		if (postProcessor.current_shader >= postProcessor.shaders.size())
			postProcessor.current_shader = -1;
	}


	if (ImGui::Button("Reset Camera"))
	{
		resetSceneCamera();
	}
}

void ThreeDeeScene::poll()
{
}

int ThreeDeeScene::AddModel(Model model)
{
	for (int i = 0; i < model.materialCount; i++)
	{
		model.materials[i].shader = lighting.shader;
	}

	model_list.push_back(model);

	return model_list.size() - 1;
}

//static int entry = SceneEntry::Register("3D II", "Shader Setup", "scripts/3D/world_one.lua", ThreeDeeScene::Create);
