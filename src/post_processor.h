#pragma once
#include <raylib/raylib.h>
#include <vector>
#include <functional>
#include <lua.hpp>

typedef std::function<void(void)>  drawFunc;


class PostProcessor
{
public:
	RenderTexture2D target;

	std::vector<Shader> shaders;
	int current_shader = -1;
public:
	PostProcessor();
	~PostProcessor();

	void AddShader(const char* fragfilepath);
	void ApplyEffect();
	void EndEffect();


public:
	static void Extend(lua_State* L);
};

