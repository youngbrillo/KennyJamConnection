#pragma once
#include <string>
#include <vector>
#include <functional>
#include <map>
#include <raylib/raylib.h>
#include <raylib/raymath.h>
#include <raylib/rlgl.h>
class Scene
{
public:
	std::string name, filePath;
public:
	Scene(std::string alias, std::string path) 
		: name(alias), filePath(path) {}
	virtual ~Scene() {}
	virtual void initialize() = 0;
	virtual void update(const float& dt) = 0;
	virtual void fixedUpdate(const float& timestep) = 0;
	virtual void predraw() = 0;
	virtual void draw() = 0;
	virtual void poll() = 0;
	virtual void Inspect() = 0;
};

typedef std::function<Scene* (std::string, std::string)>  SceneGenerator;

struct SceneEntry
{
	std::string category, name, path;
	SceneGenerator create;
	int id = 0;


	static int Register(
		std::string category,
		std::string name,
		std::string path,
		SceneGenerator func
	);
};

class SceneManager
{
public:
	static SceneManager& Get();
	static void Unload();
	static int AddScene(SceneEntry entry);
	static int AddScenesFromDirectory(const char* dirPath, const char* category, SceneGenerator gen);
	static SceneEntry* GetScene(const char* name);
	static SceneEntry* GetScene(const int& id);
	bool inspect(int& index);

private:
	SceneManager();
	~SceneManager();
	static SceneManager* instance;

	/*static*/ std::map<int, SceneEntry> Entries;
	/*static*/ std::map<std::string, int> entry_name_lookup;
	/*static*/ std::map<int, std::string> entry_name_reverse_lookup;
	/*static*/ std::map<std::string, std::vector<int>> category_entry_lookup;
	/*static*/ int sceneCount;

	friend class App;
	friend class Inspector;
	friend class AppSettings;
};
