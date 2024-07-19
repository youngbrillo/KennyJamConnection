#pragma once
#include <string>
#include <raylib/raylib.h>
#include <raylib/raymath.h>
#include <raylib/rlgl.h>

enum app_state
{
	app_state_play = 0,
	app_state_paused,
	app_state_paused_fixed
}; 

struct FrameData
{
	float currentFrame = 0.0f;
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	float fixed_interval = 1.0f / 60.0f;
	float fixed_timer = 0.0f;

	void step();
};
class Settings
{
public:
	int windowWidth = 800;
	int windowHeight = 450;

	unsigned int window_flags = 0x00000004;
	unsigned int window_color_code = 0x000000ff;
	Color window_color;
	int exit_key = 256; // KEY_ESCAPE
	int pause_key = 80; // KEY_P
	int trace_log_level = 0; //display all
	int current_scene_id = 0;
	int state = app_state::app_state_play;
	std::string windowTitle = "Rogue RPG";


	bool fullScreen = false;
	bool borderless = false;
	bool canQuit = false;
	bool canRestartScene = false;
	bool enable_autoSave = true;
	bool enable_ecs_rest = false;

	void load(const char* path = "./save.ini");
	void save(const char* path = "./save.ini");
	void tryAutoSave(const char* path = "./save.ini");
	void poll();
	void inspect();
	void drawWhenNoSceneFound();

	inline void toggleFullScreen();
	
};


inline void Settings::toggleFullScreen()
{
	fullScreen = !fullScreen;
	ClearWindowState(window_flags);
	if (fullScreen)
		window_flags |= FLAG_FULLSCREEN_MODE;
	else
		window_flags ^= FLAG_FULLSCREEN_MODE;

	SetWindowState(window_flags);
}