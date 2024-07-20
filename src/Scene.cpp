#include "Scene.h"
#include <imgui.h>
#include <raylib/raylib.h>

SceneManager* SceneManager::instance = NULL;

int SceneEntry::Register(std::string category, std::string name, std::string path, SceneGenerator func)
{
	SceneEntry e = SceneEntry{ category, name, path };
	e.create = func;
	return SceneManager::AddScene(e);
}

SceneManager& SceneManager::Get()
{
	if (instance == NULL) instance = new SceneManager();
	return *SceneManager::instance;
}

void SceneManager::Unload()
{
	if (SceneManager::instance)
	{
		delete SceneManager::instance;
	}
	SceneManager::instance = NULL;
}

int SceneManager::AddScene(SceneEntry entry)
{
	entry.id = SceneManager::Get().sceneCount++;

	SceneManager::Get().Entries[entry.id] = entry;
	SceneManager::Get().entry_name_lookup[entry.name] = entry.id;
	SceneManager::Get().entry_name_reverse_lookup[entry.id] = entry.name;
	SceneManager::Get().category_entry_lookup[entry.category].push_back(entry.id);

	return entry.id;
}

int SceneManager::AddScenesFromDirectory(const char* dirPath, const char* category, SceneGenerator gen)
{
	int count = 0;
	bool dirExists = DirectoryExists(dirPath);
	if (dirExists)
	{
		FilePathList files = LoadDirectoryFilesEx(dirPath, ".lua", false);
		for (int i = 0; i < files.count; i++)
		{
			//printf("found file %d:\n >\t%s\n", i, files.paths[i]);printf(" >\t%s\n", GetFileName(files.paths[i])); printf(" >\t%s\n", GetFileNameWithoutExt(files.paths[i]));
			int id = SceneEntry::Register(
				category, 
				GetFileNameWithoutExt(files.paths[i]), 
				files.paths[i], 
				gen
			);
			count++;
		}
		UnloadDirectoryFiles(files);
	}
	return count;
}

SceneEntry* SceneManager::GetScene(const char* name)
{
	auto query = SceneManager::Get().entry_name_lookup.find(name);
	if (query != SceneManager::Get().entry_name_lookup.end())
		return GetScene(query->second);
	return nullptr;
}

SceneEntry* SceneManager::GetScene(const int& id)
{
	auto query = SceneManager::Get().Entries.find(id);
	if (query != SceneManager::Get().Entries.end())
		return &SceneManager::Get().Entries.at(id);

	return nullptr;
}

bool SceneManager::inspect(int& index)
{
	bool r = false;
	if (ImGui::BeginMenu("Scenes"))
	{
		for (auto&& pair : category_entry_lookup)
		{
			if (ImGui::BeginMenu(pair.first.c_str()))
			{
				for (auto&& scene_id : pair.second)
				{
					if (ImGui::MenuItem(entry_name_reverse_lookup[scene_id].c_str(), TextFormat("id: %d", scene_id), index == scene_id))
					{
						r = true;
						index = scene_id;
					}
				}
				ImGui::EndMenu();
			}
		}
		ImGui::EndMenu();
	}

	return r;
}

SceneManager::SceneManager()
{
	Entries = std::map<int, SceneEntry>();
	entry_name_lookup = {};
	entry_name_reverse_lookup = {};
	category_entry_lookup = {};
	sceneCount = 0;
}

SceneManager::~SceneManager()
{
	Entries.clear();
	entry_name_lookup.clear();
	entry_name_reverse_lookup.clear();
	category_entry_lookup.clear();
}

void Scene::Render(Color& clearColor, DrawHook onInspect)
{
	BeginDrawing();
		ClearBackground(clearColor);
		DrawWorld();
		onInspect(); //<--- Scene->Inspect is called inside this
	EndDrawing();
}
