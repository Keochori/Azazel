#include "pch.h"
#include "ImGuiManager.h"
#include "Tools/Timer.h"
#include "Scene/Scene.h"
#include "Scene/Entity.h"
#include "Scene/Components/Components.h"

ImGuiManager::ImGuiManager(HWND& aHWND, ComPtr<ID3D11Device>& aDevice, ComPtr<ID3D11DeviceContext>& aContext,
	ComPtr<ID3D11ShaderResourceView>& aSceneSRV, std::unordered_map<std::string, ID3D11ShaderResourceView*>& aIcons, Scene* aScene)
	: mySceneTab(aSceneSRV), myHierarchyTab(aScene), myInspectorTab(aScene), myAssetsTab(aIcons)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(aHWND);
	ImGui_ImplDX11_Init(aDevice.Get(), aContext.Get());
}

void ImGuiManager::Shutdown()
{
	myAssetsTab.Shutdown();
}

void ImGuiManager::NewFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::DockSpaceOverViewport(0, nullptr, ImGuiDockNodeFlags_PassthruCentralNode);
	MainMenuBar();
}

void ImGuiManager::Update()
{
	//ImGui::ShowDemoWindow();
    FPSCounterTab();
	mySceneTab.Update();
	myConsoleTab.Update();
	myHierarchyTab.Update();
	myInspectorTab.Update(myHierarchyTab.GetSelectedEntity());
	myAssetsTab.Update();
}

void ImGuiManager::Render()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

const ImVec2& ImGuiManager::GetSceneTabSize()
{
	return mySceneTab.GetSceneTabSize();
}

void ImGuiManager::MainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::MenuItem("Assets"))
				myAssetsTab.OpenTab();

			if (ImGui::MenuItem("Console"))
				myConsoleTab.OpenTab();

			if (ImGui::MenuItem("Hierarchy"))
				myHierarchyTab.OpenTab();

			if (ImGui::MenuItem("Inspector"))
				myInspectorTab.OpenTab();

			if (ImGui::MenuItem("Scene"))
				mySceneTab.OpenTab();

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void ImGuiManager::FPSCounterTab()
{
	static int frameCounter = 0;
	static int currentFPS = 0;

    ImGui::Begin("FPS", nullptr, ImGuiWindowFlags_NoCollapse);
	frameCounter++;
    if (frameCounter > 30)
    {
		frameCounter = 0;
        float fps = 1.0f / TIMER.GetDeltaTime();

        float alpha = 0.25f; // lower = smoother
		currentFPS = currentFPS * (1.0f - alpha) + fps * alpha;
    }
    ImGui::Text("FPS: %i", currentFPS);
    ImGui::End();
}

