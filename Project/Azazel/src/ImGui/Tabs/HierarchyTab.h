#pragma once

class Scene;
class Entity;

class HierarchyTab
{
public:
	HierarchyTab(Scene* aScene);
	~HierarchyTab() = default;

	void Update();
	void OpenTab();
	Entity* GetSelectedEntity();

private:
	bool myTabOpen = true;

	Scene* myScene;
	Entity* mySelectedEntity = nullptr;
};

