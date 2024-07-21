#pragma once
#include <map>
#include <set>
#include <string>
#include <raylib/raylib.h>
#include <lua.hpp>
#include "util/LightingSystem.h";

class ModelManager
{
public:
	static LightingSystem lighting;
	static unsigned int cubeId, sphereId, coneId, torusId, planeId;
public:
	static void Initialize(const char* vshader = "assets/shaders/shadowmap.vs", const char* fshader = "assets/shaders/shadowmap.fs", int resolution = 1024);

	static unsigned int AddModel(const char* path);
	static unsigned int AddModelsFromDirectory(const char* dirpath, int start = 0,  int end = -1);
	static Model* GetModel(unsigned int modelId);
	static Model GetModelFlat(unsigned int modelId);
	static unsigned int GetModelCount();
	static void Clear();
	static void Extend(lua_State* L);
	
private:
	static std::map<unsigned int, Model> models;
	static std::map<std::string, unsigned int> model_lookup;
	static std::set<std::string> directories;
	static bool _is_lighting_initialized;
};



