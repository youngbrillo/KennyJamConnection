#pragma once

class Scene;
class Inspector
{
public:
	bool menu_visible = true;
	bool view_demo = false;
	bool view_stats = true;
	int corner = 1;
	bool view_scene = true;
	float scene_menu_width = 180.0f;
public:
	void initalize();
	void render();
	void poll();

private:
	void settings_menu();
	void edit_menu();
	void view_menu();
	void ecs_menu();
	void scene_select_menu();
	void current_scene_quickmenu(Scene* scene);

	void stat_overlay();
	void current_scene(Scene* scene);
};

