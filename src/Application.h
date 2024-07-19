#pragma once
#include "Settings.h"
#include "Inspector.h"
#include <lua.hpp>

class Scene;
class App
{
public:
	static Settings settings;
	static Inspector inspector;
//public facing interface no 1
public:
	//start the app
	static void Start();
	//run the app
	static void Run();
	//clean up, deallocate resources, and shut down the app
	static void End();

//life cycle interface
private:
	static void Advance();
	static void Update(const float& dt);
	static void FixedUpdate(const float& timestep);
	static void Draw();
	static void Poll();
//public facing interface no 2.
public:
	//static bool SetScene(const char* scene_alias);
	//static bool SetScene(int scene_index);
	static Settings* GetSettings() { return &App::settings; }
	static void Quit() { App::settings.canQuit = true; }
	static void Restart() { App::settings.canRestartScene = true; }
	static int GetState() { return App::settings.state; }
	static bool isPaused() { return App::settings.state != app_state::app_state_play; }
	static void Pause() { App::settings.state = app_state::app_state_paused; }
	static void PauseFull() { App::settings.state = app_state::app_state_paused_fixed; }
	static void Resume() { App::settings.state = app_state::app_state_play; }
	static void StartScene(int sceneId);
	static Scene* CurrentScene() { return current_scene; }

	static void Extend(lua_State* L);
private:
	static FrameData frameData;
	static Scene* current_scene;
	friend class Settings;
	friend class Inspector;
};

