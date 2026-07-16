#include "pch.h"
#include "FileExplorerTab.h"
#include "EditorState/EditorState.h"
#include <algorithm>
#include <fstream>
#include <cctype>

FileExplorerTab::FileExplorerTab(std::unordered_map<std::string, ID3D11ShaderResourceView*>& aIcons)
{
	myDirectoryTree = new DirectoryTree(myLeftPanelFocused, aIcons);
}

void FileExplorerTab::Shutdown()
{
	myDirectoryTree->Shutdown();
}

void FileExplorerTab::Update()
{
	if (myTabOpen)
	{
		ImGui::Begin("File Explorer", &myTabOpen, ImGuiWindowFlags_NoCollapse);

		LeftPanel();
		PanelSplitter();

		ImGui::End();
	}
}

void FileExplorerTab::OpenTab()
{
	myTabOpen = true;
}

void FileExplorerTab::LeftPanel()
{
	ImGui::BeginChild("LeftPanel", ImVec2(myLeftPanelWidth, 0), true);

	myLeftPanelFocused = ImGui::IsWindowFocused();
	myDirectoryTree->Draw();

	ImGui::EndChild();

	myDirectoryTree->Update();
}

void FileExplorerTab::PanelSplitter()
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
