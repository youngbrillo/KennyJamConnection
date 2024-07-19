#include "Settings.h"
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "Application.h"
static std::vector<std::string> getFileAttributes(const std::string& line, char delimiter)
{
	std::vector<std::string> arr;
	std::stringstream ss(line);
	std::string token;
	while (std::getline(ss, token, delimiter))
	{
		arr.push_back(token);
	}

	if (arr.size() < 2) {
		arr.push_back("");
		arr.push_back("");
	}
	return arr;
}
void Settings::load(const char* path)
{
	std::ifstream file(path);
	std::string line;
	if (file.is_open())
	{
		int i = 0;
		while (std::getline(file, line))
		{
			//std::cout << "line: "<< ++i << line << std::endl;
			auto arr = getFileAttributes(line, '=');
			std::string _attribute = arr[0];
			std::string _value = arr[1];
			if (_attribute.compare("windowTitle") == 0)
				windowTitle = _value;
			else if (_attribute.compare("windowWidth") == 0)
				windowWidth = std::stoi(_value);
			else if (_attribute.compare("windowHeight") == 0)
				windowHeight = std::stoi(_value);
			else if (_attribute.compare("window_flags") == 0)
				window_flags = std::stoi(_value);
			else if (_attribute.compare("trace_log_level") == 0)
				trace_log_level = std::stoi(_value);
			else if (_attribute.compare("window_color_code") == 0) {
				std::istringstream converter(_value);
				converter >> std::hex >> window_color_code;
			}
			else if (_attribute.compare("enable_autoSave") == 0)
				enable_autoSave = std::stoi(_value);
			else if (_attribute.compare("current_scene_id") == 0)
				current_scene_id = std::stoi(_value);
			else if (_attribute.compare("inspector.menu_visible") == 0)  
				App::inspector.menu_visible = std::stoi(_value);
			else if (_attribute.compare("inspector.view_scene") == 0)  
				App::inspector.view_scene = std::stoi(_value);
			else if (_attribute.compare("inspector.view_stats") == 0)  
				App::inspector.view_stats = std::stoi(_value);
			else if (_attribute.compare("enable_ecs_rest") == 0)
				enable_ecs_rest = std::stoi(_value);
			else if (_attribute.compare("fullScreen") == 0)
				fullScreen = std::stoi(_value);
			else if (_attribute.compare("borderless") == 0)
				borderless = std::stoi(_value);
		}
		file.close();
	}

	window_color = GetColor(window_color_code);
}

void Settings::save(const char* path)
{

	std::ofstream file(path);

	file << "windowTitle=" << windowTitle << std::endl;
	file << "windowWidth=" << GetScreenWidth() << std::endl;
	file << "windowHeight=" << GetScreenHeight() << std::endl;
	file << "window_flags=" << window_flags << std::endl;
	file << "trace_log_level=" << trace_log_level << std::endl;

	std::stringstream hexValue;
	hexValue.imbue(std::locale("C"));
	hexValue << std::uppercase << std::hex << window_color_code;
	file << "window_color_code=" << hexValue.str() << std::endl;
	file << "enable_autoSave=" << enable_autoSave << std::endl;
	file << "current_scene_id=" << current_scene_id << std::endl;
	file << "inspector.menu_visible=" << App::inspector.menu_visible << std::endl;
	file << "inspector.view_scene=" << App::inspector.view_scene << std::endl;
	file << "inspector.view_stats=" << App::inspector.view_stats << std::endl;
	file << "enable_ecs_rest=" << enable_ecs_rest << std::endl;
	file << "fullScreen=" << fullScreen << std::endl;
	file << "borderless=" << borderless << std::endl;

	file.close();
}

void Settings::tryAutoSave(const char* path)
{
	if (!enable_autoSave) return;
	save(path);
}

void Settings::poll()
{
}

void Settings::inspect()
{
}

void Settings::drawWhenNoSceneFound()
{
	Vector2 pos = { GetScreenWidth() * 0.5f, GetScreenHeight() * 0.5f };
	const char* message = "No Scene Found";
	float fontSize = 35.0f;
	Vector2 size = MeasureTextEx(GetFontDefault(), message, fontSize, 1.0f);
	size = Vector2Scale(size, 0.5f);
	DrawTextPro(GetFontDefault(), message, pos, size, 0.0f, fontSize, 1.0f, WHITE);
}

void FrameData::step()
{
	currentFrame = (float)glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}
