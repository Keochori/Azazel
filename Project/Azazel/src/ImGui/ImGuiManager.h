#pragma once
#include "ImGui/imguiIncludes.h"
#include "Tabs/SceneTab.h"
#include "Tabs/ConsoleTab.h"
#include "Tabs/HierarchyTab.h"
#include "Tabs/InspectorTab.h"
#include "Tabs/AssetsTab/AssetsTab.h"

using namespace Microsoft::WRL;

class Scene;

class ImGuiManager
{
public:
	ImGuiManager(HWND& aHWND, ComPtr<ID3D11Device>& aDevice, ComPtr<ID3D11DeviceContext>& aContext, 
		ComPtr<ID3D11ShaderResourceView>& aSceneSRV, std::unordered_map<std::string, ID3D11ShaderResourceView*>& aIcons, Scene* aScene);
	~ImGuiManager() = default;

	void Shutdown();
	void NewFrame();
	void Update();
	void Render();

	const ImVec2& GetSceneTabSize();

	void MainMenuBar();
	void FPSCounterTab();

private:
	SceneTab mySceneTab;
	ConsoleTab myConsoleTab;
	HierarchyTab myHierarchyTab;
	InspectorTab myInspectorTab;
	AssetsTab myAssetsTab;
};
