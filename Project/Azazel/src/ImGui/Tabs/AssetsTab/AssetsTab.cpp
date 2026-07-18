#include "pch.h"
#include "AssetsTab.h"
#include "EditorState/EditorState.h"
#include <algorithm>
#include <fstream>
#include <cctype>

AssetsTab::AssetsTab(std::unordered_map<std::string, ID3D11ShaderResourceView*>& aIcons)
{
	myDirectoryTree = new DirectoryTree(myLeftPanelFocused, aIcons);
}

void AssetsTab::Shutdown()
{
	myDirectoryTree->Shutdown();
}

void AssetsTab::Update()
{
	if (myTabOpen)
	{
		ImGui::Begin("Assets", &myTabOpen, ImGuiWindowFlags_NoCollapse);

		LeftPanel();
		PanelSplitter();

		ImGui::End();
	}
}

void AssetsTab::OpenTab()
{
	myTabOpen = true;
}

void AssetsTab::LeftPanel()
{
	ImGui::BeginChild("LeftPanel", ImVec2(myLeftPanelWidth, 0), true);

	myLeftPanelFocused = ImGui::IsWindowFocused();
	myDirectoryTree->Draw();

	ImGui::EndChild();

	myDirectoryTree->Update();
}

void AssetsTab::PanelSplitter()
{
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.2f, 0.2f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 0.3f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.5f, 0.5f, 0.5f));
	ImGui::Button("##Splitter", ImVec2(6.0f, -1));
	ImGui::PopStyleColor(3);

	const float minWidth = 100.0f;
	float maxWidth = ImGui::GetWindowSize().x * 0.8f;

	if (maxWidth < minWidth)
		maxWidth = minWidth;

	if (ImGui::IsItemActive())
	{
		myLeftPanelWidth += ImGui::GetIO().MouseDelta.x;
		myLeftPanelWidth = std::clamp(myLeftPanelWidth, minWidth, maxWidth);
	}
	ImGui::SameLine();
}
