#include "ModelManager.h"
#include <iostream>
#include <raylib/raymath.h>
#include <raylib/rlgl.h>

std::map<unsigned int, Model> ModelManager::models = {};
std::map<std::string, unsigned int> ModelManager::model_lookup = {};
std::set<std::string> ModelManager::directories = {};
Model* ModelManager::AddModel(const char* path)
{
	if (!FileExists(path))
		return nullptr;

	std::string fileName = GetFileNameWithoutExt(path);
	if (model_lookup.find(fileName) != model_lookup.end())
	{
		return GetModel(model_lookup.at(fileName));
	}
	unsigned int next_id = GetModelCount();
	models.emplace(next_id, LoadModel(path));
	model_lookup.emplace(fileName, next_id);
	return &models.at(next_id);
}

unsigned int ModelManager::AddModelsFromDirectory(const char* dirpath, int start, int end)
{
	if (directories.find(dirpath) != directories.end()) return 0;

	bool dirExists = DirectoryExists(dirpath);
	if (!dirExists) return 0;
	if(start == 0 && end == -1)
		directories.emplace(dirpath);


	FilePathList files = LoadDirectoryFilesEx(dirpath, ".obj", false);

	int begin = start >= 0 && start < end&& start < files.count - 1 ? start : 0;
	int limit = end > 0 ? 
				end <= files.count ? 
						end 
						: files.count 
				: files.count;

	for (int i = begin; i < limit; i++)
	{
		unsigned int next_id = GetModelCount();

		//std::cout << next_id << ": " << files.paths[i] << " [" << i << "]" << std::endl;
#if true
		Model model = LoadModel(files.paths[i]);
		models.emplace(next_id, model);
		model_lookup.emplace(GetFileNameWithoutExt(files.paths[i]), next_id);
#else
		ModelManager::AddModel(files.paths[i]);
#endif
	}
	UnloadDirectoryFiles(files);
	return GetModelCount()-1;
}

Model* ModelManager::GetModel(unsigned int index)
{
	if(models.find(index) == models.end())
		return nullptr;

	return &models.at(index);
}

Model ModelManager::GetModelFlat(unsigned int modelId)
{
	return *GetModel(modelId);
}

unsigned int ModelManager::GetModelCount()
{
	return ModelManager::models.size();
}

void ModelManager::Clear()
{
	models.clear();
	model_lookup.clear();
	directories.clear();
}

#include <LuaBridge/LuaBridge.h>
void ModelManager::Extend(lua_State* L)
{
	luabridge::getGlobalNamespace(L)
		.beginClass<ModelManager>("ModelManager")
		.addStaticFunction("AddModel", ModelManager::AddModel)
		.addStaticFunction("GetModel", ModelManager::GetModel)
		.addStaticFunction("GetModelFlat", ModelManager::GetModelFlat)
		.addStaticFunction("GetModelCount", ModelManager::GetModelCount)
		.addStaticFunction("AddModelsFromDirectory", ModelManager::AddModelsFromDirectory)
		.addStaticFunction("Clear", ModelManager::Clear)
		.endClass();
}
