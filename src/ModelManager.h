#pragma once
#include <map>
#include <set>
#include <string>
#include <raylib/raylib.h>
#include <lua.hpp>

class ModelManager
{
public:
	static Model* AddModel(const char* path);
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
};



