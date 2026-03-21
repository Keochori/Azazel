#include "pch.h"
#include "ImGuiManager.h"
#include "ImGui/imguiIncludes.h"
#include "Tools/Timer.h"

ImGuiManager::ImGuiManager(HWND& aHWND, ComPtr<ID3D11Device>& aDevice, ComPtr<ID3D11DeviceContext>& aContext)
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

void ImGuiManager::NewFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGuiManager::Update()
{
	ImGui::ShowDemoWindow();
	myFrameCounter++;
	if (myFrameCounter > 30)
	{
		myFrameCounter = 0;
		float fps = 1.0f / TIMER.GetDeltaTime();

		float alpha = 0.25f; // lower = smoother
		myCurrentFPS = myCurrentFPS * (1.0f - alpha) + fps * alpha;
	}
	ImGui::Text("FPS: %i", myCurrentFPS);
}

void ImGuiManager::Render()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}