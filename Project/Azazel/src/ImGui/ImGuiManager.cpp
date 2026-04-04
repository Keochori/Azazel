#include "pch.h"
#include "ImGuiManager.h"
#include "Tools/Timer.h"
#include "Scene/Scene.h"
#include "Scene/Entity.h"
#include "Scene/Components/Components.h"

ImGuiManager::ImGuiManager(HWND& aHWND, ComPtr<ID3D11Device>& aDevice, ComPtr<ID3D11DeviceContext>& aContext,
	ComPtr<ID3D11ShaderResourceView>& aTextureSRV, Scene* aScene) : myTextureSRV(aTextureSRV), myScene(aScene)
{
	mySelectedEntity = nullptr;

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

	// Register components to draw
	RegisterComponentToDraw<TransformComponent>("Transform");
	RegisterComponentToDraw<MeshComponent>("Mesh");
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
	SceneTab();
	HierarchyTab();
	InspectorTab();
	AssetsTab();
	ConsoleTab();
}

void ImGuiManager::Render()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

const ImVec2& ImGuiManager::GetSceneTabSize()
{
	return mySceneTabSize;
}

void ImGuiManager::MainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			//if (ImGui::MenuItem("Open")) 
			//{ 

			//}
			//if (ImGui::MenuItem("Save")) 
			//{

			//}
			//ImGui::Separator();
			//if (ImGui::MenuItem("Exit")) 
			//{

			//}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			//if (ImGui::MenuItem("Scene Fullscreen", "F"))
			//{
			//	
			//}

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void ImGuiManager::FPSCounterTab()
{
    ImGui::Begin("FPS", nullptr, ImGuiWindowFlags_NoCollapse);
    myFrameCounter++;
    if (myFrameCounter > 30)
    {
        myFrameCounter = 0;
        float fps = 1.0f / TIMER.GetDeltaTime();

        float alpha = 0.25f; // lower = smoother
        myCurrentFPS = myCurrentFPS * (1.0f - alpha) + fps * alpha;
    }
    ImGui::Text("FPS: %i", myCurrentFPS);
    ImGui::End();
}

void ImGuiManager::SceneTab()
{
	ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoCollapse);
	mySceneTabSize = ImGui::GetContentRegionAvail();
	ImGui::Image((void*)myTextureSRV.Get(), mySceneTabSize);
	ImGui::End();
}

void ImGuiManager::HierarchyTab()
{
	ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_NoCollapse);

	bool anyItemHovered = false;

	for (Entity& entity : myScene->GetEntities())
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf;
		bool isSelected = (mySelectedEntity == &entity); 
		
		if (isSelected)
			flags |= ImGuiTreeNodeFlags_Selected;

		if (ImGui::TreeNodeEx(entity.GetName().c_str(), flags))
			ImGui::TreePop();

		if (ImGui::IsItemClicked())
			mySelectedEntity = &entity;

		if (ImGui::IsItemHovered())
			anyItemHovered = true;
	}

	if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(0) && !anyItemHovered)
		mySelectedEntity = nullptr;

    ImGui::End();
}

void ImGuiManager::InspectorTab()
{
	ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_NoCollapse);

	if (mySelectedEntity != nullptr)
		for (auto& drawFn : myComponentsToDraw)
			drawFn(myScene->GetRegistry(), mySelectedEntity->GetHandle());

	ImGui::End();
}

void ImGuiManager::AssetsTab()
{
	ImGui::Begin("Asets", nullptr, ImGuiWindowFlags_NoCollapse);

	ImGui::End();
}

void ImGuiManager::ConsoleTab()
{
	ImGui::Begin("Console", nullptr, ImGuiWindowFlags_NoCollapse);

	ImGui::End();
}

void ImGuiManager::DrawComponentUI(TransformComponent& aTransformComponent)
{
	ImGui::DragFloat3("Position", &aTransformComponent.myTransform.myPosition.x, 0.1f);
}

void ImGuiManager::DrawComponentUI(MeshComponent& aMeshComponent)
{
	
}
