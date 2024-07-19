#include "Application.h"
#include "Scene.h"
#include <raylib/raylib.h>
#include <GLFW/glfw3.h>
//#include "src/managers/ResourceManager.h"

Settings App::settings = Settings();
FrameData App::frameData = FrameData();
Inspector App::inspector = Inspector();

Scene* App::current_scene = NULL;

void App::Start()
{
	settings.load();
	SetTraceLogLevel(settings.trace_log_level);
	InitWindow(settings.windowWidth, settings.windowHeight, settings.windowTitle.c_str());
	SetWindowState(settings.window_flags);
	SetExitKey(settings.exit_key);
	
	//resources::InitializeResources();
	StartScene(settings.current_scene_id);
	inspector.initalize();
}

void App::Run()
{
	while (!settings.canQuit)
	{
		App::Advance();
		App::Draw();
		App::Poll();
	}
}

void App::End()
{
	if (current_scene)
	{
		delete current_scene;
		current_scene = NULL;
	}
	settings.tryAutoSave();
	//resources::CleanupResources();
}

void App::Advance()
{
	frameData.step();
	App::Update(frameData.deltaTime);
}

void App::Update(const float& dt)
{
	if (settings.state == app_state::app_state_paused)
		return;
	if (current_scene) current_scene->update(dt);

	
	frameData.fixed_timer += dt;
	if (frameData.fixed_timer >= frameData.fixed_interval)
	{
		App::FixedUpdate(frameData.fixed_interval);
		frameData.fixed_timer = 0.0f;
	}
}

void App::FixedUpdate(const float& timestep)
{
	if (settings.state == app_state::app_state_paused_fixed)
		return;
	if (current_scene) current_scene->fixedUpdate(timestep);
}

void App::Draw()
{
	BeginDrawing();
	ClearBackground(settings.window_color);
	if (current_scene) 
		current_scene->draw();
	else 
		settings.drawWhenNoSceneFound();

	inspector.render();
	EndDrawing();
}

void App::Poll()
{
	if (WindowShouldClose()) settings.canQuit = true;
	if (current_scene) current_scene->poll();
	settings.poll();
	inspector.poll();

	if (settings.canRestartScene)
	{
		App::StartScene(settings.current_scene_id);
	}
}

void App::StartScene(int sceneId)
{
	settings.current_scene_id = sceneId;
	if (!!current_scene)
	{
		delete current_scene;
		current_scene = NULL;
		settings.tryAutoSave();
	}

	SceneEntry* entry = SceneManager::GetScene(sceneId);
	settings.canRestartScene = false;
	settings.state = app_state::app_state_play;

	if (!!entry)
	{
		current_scene = entry->create(entry->name, entry->path);
		current_scene->initialize();
	}

}

#include <LuaBridge/LuaBridge.h>
void App::Extend(lua_State* L)
{
	//resources::ExtendResources(L);
	luabridge::getGlobalNamespace(L)
		.beginClass<App>("App")
		//.addStaticProperty("currentScene", App::current_scene)
		//.addStaticData("settings", App::GetSettings, false)
		.addStaticFunction("Restart", App::Restart)
		.addStaticFunction("Quit", App::Quit)
		.addStaticFunction("GetState", App::GetState)
		.addStaticFunction("isPaused", App::isPaused)
		.addStaticFunction("Pause", App::Pause)
		.addStaticFunction("PauseFull", App::PauseFull)
		.addStaticFunction("Resume", App::Resume)
		.addStaticFunction("StartScene", App::StartScene)
		//.addStaticFunction("GotoScene", App::GotoScene)
		//.addStaticFunction("GotoSceneId", App::GotoSceneId)
		.endClass();
}
