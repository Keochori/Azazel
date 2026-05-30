#pragma once
#include "ImGui/imguiIncludes.h"
#include "entt/entt.hpp"

// [HOW TO REGISTER A COMPONENT TO BE DRAWN]:
// 1. Forward declare the component type
// 2. Create a new overloaded version for the component (void DrawComponentUI)
// 3. Register the component to be drawn using RegisterComponentToDraw() in the constructor

class Scene;
class Entity;

// Forward Declare for every registered component
struct TransformComponent;
struct ModelComponent;

class InspectorTab
{
public:
	InspectorTab(Scene* aScene);
	~InspectorTab() = default;

	void Update(Entity* aEntity);
	void OpenTab();

private:
	// Component draw logic
	template <typename T>
	void RegisterComponentToDraw(const std::string& aName);

	// Add and define for every registered component
	void DrawComponentUI(TransformComponent& aTransformComponent);
	void DrawComponentUI(ModelComponent& aModelComponent);

	bool myTabOpen = true;

	Scene* myScene;
	Entity* mySelectedEntity = nullptr;

	std::vector<std::function<void(entt::registry&, const entt::entity&)>> myComponentsToDraw;
};

template <typename T>
void InspectorTab::RegisterComponentToDraw(const std::string& aName)
{
	myComponentsToDraw.emplace_back([&, aName](entt::registry& aRegistry, const entt::entity& aHandle)
		{
			if (!aRegistry.any_of<T>(aHandle))
				return;

			if (ImGui::CollapsingHeader(aName.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
			{
				auto& component = aRegistry.get<T>(aHandle);
				DrawComponentUI(component);
			}
		});
}
