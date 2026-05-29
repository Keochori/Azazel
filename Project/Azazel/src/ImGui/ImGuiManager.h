#pragma once
#include "ImGui/imguiIncludes.h"
#include "Tabs/FileExplorerTab.h"
#include "entt/entt.hpp"
#include <filesystem>
#include <functional>
#include <d3d11.h>
#include <memory>
#include <wrl.h> 

// [HOW TO REGISTER A COMPONENT TO BE DRAWN]:
// 1. Forward declare the component type
// 2. Create a new overloaded version for the component (void DrawComponentUI)
// 3. Register the component to be drawn using RegisterComponentToDraw() in the constructor

using namespace Microsoft::WRL;

class Scene;
class Entity;

// Forward Declare for every registered component
struct TransformComponent;
struct ModelComponent;

class ImGuiManager
{
public:
	ImGuiManager(HWND& aHWND, ComPtr<ID3D11Device>& aDevice, ComPtr<ID3D11DeviceContext>& aContext, 
		ComPtr<ID3D11ShaderResourceView>& aTextureSRV, std::unordered_map<std::string, ID3D11ShaderResourceView*>& aIcons, Scene* aScene);
	~ImGuiManager() = default;

	void NewFrame();
	void Update();
	void Render();
	const ImVec2& GetSceneTabSize();

	void MainMenuBar();
	void FPSCounterTab();
	void SceneTab();
	void HierarchyTab();
	void InspectorTab();
	void ConsoleTab();

	// Component draw logic
	template <typename T>
	void RegisterComponentToDraw(const std::string& aName);

	// Add and define for every registered component
	void DrawComponentUI(TransformComponent& aTransformComponent);
	void DrawComponentUI(ModelComponent& aModelComponent);

private:
	ComPtr<ID3D11ShaderResourceView>& myTextureSRV; // Texture that the world gets rendered into
	ImVec2 mySceneTabSize;

	int myFrameCounter = 0;
	int myCurrentFPS = 0;

	Scene* myScene;
	Entity* mySelectedEntity = nullptr;

	FileExplorerTab myFileExplorerTab;

	std::vector<std::function<void(entt::registry&, const entt::entity&)>> myComponentsToDraw;
};

template <typename T>
void ImGuiManager::RegisterComponentToDraw(const std::string& aName)
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
