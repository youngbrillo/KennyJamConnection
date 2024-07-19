#include "Inspector.h"
#include "util/rlImGui.h"
#include "Application.h"
#include "Scene.h"
#include <raylib/raylib.h>
//#include "src/managers/ResourceManager.h"

//#include "TextureManager.h"
//#include "BoxSystem.h"
//#include "FontManager.h"

void Inspector::initalize()
{
	rlImGuiSetup(true);

}

void Inspector::render()
{

	if (!menu_visible) return;

	rlImGuiBegin();

	if (ImGui::BeginMainMenuBar())
	{
		settings_menu();
		edit_menu();
		view_menu();
		//ecs_menu();
		scene_select_menu();
		if (App::current_scene) current_scene_quickmenu(App::current_scene);
		ImGui::EndMainMenuBar();
	}
	if (this->view_demo) ImGui::ShowDemoWindow(&this->view_demo);
	if (view_stats) stat_overlay();
	if (App::current_scene) current_scene(App::current_scene);
	rlImGuiEnd();
}

void Inspector::poll()
{
	if (IsKeyReleased(KEY_F1))
	{
		menu_visible = !menu_visible;
	}
	if (IsKeyReleased(KEY_F2))
	{
		view_scene = !view_scene;
	}

	if (IsKeyReleased(KEY_F3))
	{
		view_stats = !view_stats;
	}
	if (IsKeyPressed(App::settings.pause_key))
	{
		App::settings.state = App::settings.state == app_state_play ? app_state_paused : app_state_play;
	}
	if (IsKeyReleased(KEY_R) && IsKeyDown(KEY_LEFT_CONTROL))
	{
		App::settings.canRestartScene = true;
	}
	if (IsKeyReleased(KEY_S) && IsKeyDown(KEY_LEFT_CONTROL))
	{
		App::settings.save();
	}
	if (IsKeyReleased(KEY_F11))
	{
		App::settings.toggleFullScreen();
	}
}

void Inspector::settings_menu()
{
	if (ImGui::BeginMenu("File"))
	{
		ImGui::MenuItem("New", NULL, false, false);
		if (ImGui::MenuItem("Open", "Ctrl+O", false, false)) {}
		if (ImGui::MenuItem("Save", "Ctrl+S")) {
			App::settings.save();
		}
		if (ImGui::MenuItem("Save As..", NULL, false, false)) {}
		if (ImGui::MenuItem("Restart", "Ctrl+R", false, true)) {
			App::settings.canRestartScene = true;
		}

		ImGui::Separator();
		if (ImGui::MenuItem("Quit", "Alt+F4")) {
			App::settings.canQuit = true;
		}
		ImGui::EndMenu();
	}
}

static const char* logLevels[] = { "LOG_ALL", "LOG_TRACE" , "LOG_DEBUG" , "LOG_INFO" , "LOG_WARNING" , "LOG_ERROR" , "LOG_FATAL" , "LOG_NONE" };
void Inspector::edit_menu()
{
	if (ImGui::BeginMenu("Edit"))
	{
		if (ImGui::MenuItem("AutoSave", "", App::settings.enable_autoSave)) 
			App::settings.enable_autoSave = !App::settings.enable_autoSave;

		if (ImGui::BeginMenu("Log Levels"))
		{
			for (int i = 0; i < LOG_NONE; i++)
			{
				if (ImGui::MenuItem(logLevels[i], NULL, App::settings.trace_log_level == i))
				{
					App::settings.trace_log_level = i;
					SetTraceLogLevel(App::settings.trace_log_level);
				}
			}
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("ECS REST enabled", "", App::settings.enable_ecs_rest))
			App::settings.enable_ecs_rest = !App::settings.enable_ecs_rest;

		ImGui::EndMenu();
	}
}

void Inspector::view_menu()
{
	if (ImGui::BeginMenu("View"))
	{
		if (ImGui::MenuItem("Toggle Menu", "F1")) {
			this->menu_visible = !this->menu_visible;
		}
		if (ImGui::MenuItem("Toggle Settings Menu", "F2")) {
			this->view_scene = !this->view_scene;
		}
		if (ImGui::MenuItem("Status Overlay", "F3")) {
			this->view_stats = !this->view_stats;
		}
		if (ImGui::MenuItem(view_demo ? "Hide Demo" : "View Demo", NULL, view_demo)) {
			this->view_demo = !this->view_demo;
		}
		if (ImGui::MenuItem("FullScreen", "F11")) {
			App::settings.toggleFullScreen();
		}
		ImGui::EndMenu();
	}
}

void Inspector::ecs_menu()
{
	if (ImGui::BeginMenu("ECS", false))
	{

		ImGui::EndMenu();
	}
}
void Inspector::scene_select_menu()
{
	if (SceneManager::Get().inspect(App::settings.current_scene_id))
	{
		App::Restart();
	}
}

void Inspector::current_scene_quickmenu(Scene* scene)
{

	if (ImGui::BeginMenu(scene->name.c_str(), false))
	{

		ImGui::EndMenu();
	}
}


void Inspector::stat_overlay()
{
	ImGuiIO& io = ImGui::GetIO();
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
	if (corner != -1)
	{
		const float PAD = 10.0f;
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
		ImVec2 work_size = viewport->WorkSize;
		ImVec2 window_pos, window_pos_pivot;
		window_pos.x = (corner & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
		window_pos.y = (corner & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
		window_pos_pivot.x = (corner & 1) ? 1.0f : 0.0f;
		window_pos_pivot.y = (corner & 2) ? 1.0f : 0.0f;
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
		window_flags |= ImGuiWindowFlags_NoMove;
	}
	ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
	if (ImGui::Begin("Status Menu", &view_stats, window_flags))
	{
		//IMGUI_DEMO_MARKER("Examples/Simple Overlay");
		ImGui::Text("Staus Menu overlay\n" "in the corner of the screen.\n" "(right-click to change position)");
		ImGui::Text("%.3f ms/frame (%04.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("State: %s", App::settings.state == app_state_play ? "Running" :
			App::settings.state == app_state_paused_fixed ? "Fixed Pause" : "Paused"
		);
		ImGui::Separator();
		/*
		if (ImGui::IsMousePosValid())
			ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
		else
			ImGui::Text("Mouse Position: <invalid>");
		*/
		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::MenuItem("Custom", NULL, corner == -1)) corner = -1;
			if (ImGui::MenuItem("Top-left", NULL, corner == 0)) corner = 0;
			if (ImGui::MenuItem("Top-right", NULL, corner == 1)) corner = 1;
			if (ImGui::MenuItem("Bottom-left", NULL, corner == 2)) corner = 2;
			if (ImGui::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;
			if (view_stats && ImGui::MenuItem("Close")) view_stats = false;
			ImGui::EndPopup();
		}
	}
	ImGui::End();
}
void Inspector::current_scene(Scene* scene)
{
	if (!view_scene) return;
	/*
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImVec2 work_size = viewport->WorkSize;

	ImGui::SetNextWindowPos({ work_size.x - scene_menu_width - 10.0f, 10.0f});
	ImGui::SetNextWindowSize({ scene_menu_width, work_size.y - 20.0f});
	*/
	int flags = 0;
	//flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
	ImGui::Begin("Settings", &view_scene, flags);
	if (ImGui::BeginTabBar("Settings_tabbar", ImGuiTabBarFlags_None))
	{
		//resources::InspectResources();
		if (ImGui::BeginTabItem(scene->name.c_str()))
		{
			if (App::current_scene) App::current_scene->Inspect();

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("App"))
		{
			App::settings.inspect();
			//TextureManager::Inspect();
			//FontManager::Inspect();
			//BoxSystem::Inspect();

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Script"))
		{

			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}


	ImGui::End();
}