#include "pch.h"
#include "ImGuiManager.h"
#include "Tools/Timer.h"
#include "Scene/Scene.h"
#include "Scene/Entity.h"
#include "Scene/Components/Components.h"

ImGuiManager::ImGuiManager(HWND& aHWND, ComPtr<ID3D11Device>& aDevice, ComPtr<ID3D11DeviceContext>& aContext, Scene* aScene) : myScene(aScene)
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
}

void ImGuiManager::Update()
{
	//ImGui::ShowDemoWindow();
    FPSCounterTab();
	HierarchyTab();
	InspectorTab();
}

void ImGuiManager::Render()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiManager::FPSCounterTab()
{
    ImGui::Begin("FPS");
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

void ImGuiManager::DrawComponentUI(TransformComponent& aTransformComponent)
{
	ImGui::DragFloat3("Position", &aTransformComponent.myTransform.myPosition.x, 0.1f);
}

void ImGuiManager::DrawComponentUI(MeshComponent& aMeshComponent)
{
	
}
