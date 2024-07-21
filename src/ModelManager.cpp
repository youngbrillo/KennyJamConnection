#include "ModelManager.h"
#include <iostream>
#include <raylib/raymath.h>
#include <raylib/rlgl.h>
#include <vector>

std::map<unsigned int, Model> ModelManager::models = {};
std::map<std::string, unsigned int> ModelManager::model_lookup = {};
std::set<std::string> ModelManager::directories = {};
//global lighting
LightingSystem ModelManager::lighting = LightingSystem();
bool ModelManager::_is_lighting_initialized = false;
unsigned int ModelManager::cubeId = 0;
unsigned int ModelManager::sphereId = 0;
unsigned int ModelManager::coneId = 0;
unsigned int ModelManager::torusId = 0;
unsigned int ModelManager::planeId = 0;



//fill out primatives
void ModelManager::Initialize(const char* vs, const char* fs, int resolution)
{
	if (_is_lighting_initialized) return;

	struct mesh_maker
	{
		Mesh mesh;
		const char* name;
		unsigned int *id;
	};
	lighting.Initialize(vs, fs, resolution);
	std::vector<mesh_maker> meshList = {
		{ GenMeshCube(1.0f, 1.0f, 1.0f), "cube", &cubeId},
		{ GenMeshSphere(0.5f, 16, 16), "sphere", &sphereId},
		{ GenMeshCone(0.5f, 1.0f, 16), "cone", &coneId},
		{ GenMeshTorus(0.4f, 1.0f, 16, 32), "torus", &torusId},
		{ GenMeshPlane(1.0f, 1.0f, 16,16), "plane", &planeId}
	};



	for (int i = 0; i < meshList.size(); i++)
	{
		Model model = LoadModelFromMesh(meshList[i].mesh);
		for (int ii = 0; ii < model.materialCount; ii++)
		{
			model.materials[ii].shader = lighting.shader;
		}
		models.emplace(i, model);
		model_lookup.emplace(meshList[i].name, i);
		*meshList[i].id = i;
	}

	_is_lighting_initialized = true;
}


unsigned int ModelManager::AddModel(const char* path)
{
	if (!FileExists(path))
		return 0;

	std::string fileName = GetFileNameWithoutExt(path);
	if (model_lookup.find(fileName) != model_lookup.end())
	{
		return model_lookup.at(fileName);
	}
	unsigned int next_id = GetModelCount();
	Model model = LoadModel(path);
	models.emplace(next_id, model);
	model_lookup.emplace(fileName, next_id);


	for (int i = 0; i < model.materialCount; i++)
	{
		model.materials[i].shader = lighting.shader;
	}

	return next_id;
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

		for (int i = 0; i < model.materialCount; i++)
		{
			model.materials[i].shader = lighting.shader;
		}
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
	_is_lighting_initialized = false;
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
