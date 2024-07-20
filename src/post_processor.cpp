#include "post_processor.h"
#include "Application.h"

PostProcessor::PostProcessor()
{
	target = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
}

PostProcessor::~PostProcessor()
{
	for (auto& shader : shaders)
	{
		UnloadShader(shader);
	}

	UnloadRenderTexture(target);
}

void PostProcessor::AddShader(const char* fragfilepath)
{
	Shader shader = LoadShader(0, fragfilepath);
	shaders.emplace_back(shader);
	current_shader++;
}

void PostProcessor::onBeginDraw()
{
	if (current_shader <= -1 || current_shader >= shaders.size())
		return;
	//EndDrawing();
	BeginTextureMode(target);       // Enable drawing to texture
	ClearBackground(App::settings.window_color);

}
void PostProcessor::onEndDraw()
{
	if (current_shader <= -1 || current_shader >= shaders.size())
		return;

	EndTextureMode();
	//BeginDrawing();

	BeginShaderMode(shaders[current_shader]);
	DrawTextureRec(
		target.texture,
		//Rectangle{ 0, 0, (float)target.texture.width, (float)-target.texture.height },
		Rectangle{ 0, 0, GetScreenWidth() * 1.0f, GetScreenHeight() * -1.0f},
		Vector2{ 0.0f, 0.0f }, WHITE
	);
	EndShaderMode();
}

#include <LuaBridge/LuaBridge.h>
void PostProcessor::Extend(lua_State* L)
{
	luabridge::getGlobalNamespace(L)
		.beginClass<PostProcessor>("PostProcessor")
			.addData("current_shader", &PostProcessor::current_shader)
			.addFunction("AddShader", &PostProcessor::AddShader)
		.endClass();
}
