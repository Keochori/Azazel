#include "pch.h"
#include "FileExplorerTab.h"
#include <algorithm>

FileExplorerTab::FileExplorerTab(std::unordered_map<std::string, ID3D11ShaderResourceView*>& aIcons)
{
	mySelectedPaths.insert("Assets");

	myFolderIcon_Closed = aIcons.at("folder_closed");
	myFolderIcon_Open = aIcons.at("folder_open");
	myFolderIcon_Empty = aIcons.at("folder_empty");
	myArrowIcon_Right = aIcons.at("arrow_right");
	myArrowIcon_Down = aIcons.at("arrow_down");
}

void FileExplorerTab::Update()
{
	if (myTabOpen)
	{
		ImGui::Begin("File Explorer", &myTabOpen, ImGuiWindowFlags_NoCollapse);

		// Left Panel (Directory Tree)
		ImGui::BeginChild("LeftPanel", ImVec2(myLeftPanelWidth, 0), true);
		DrawDirectoryTree(myAssetsPath, 0);
		ImGui::EndChild();

		// Panel Splitter
		PanelSplitter();

		// Right Panel (File Explorer)
		ImGui::BeginChild("RightPanel", ImVec2(0, 0), true);
		for (auto& path : mySelectedPaths)
			ImGui::Text(path.string().c_str());
		ImGui::EndChild();

		// Check for Pending Move
		UpdatePendingMove();

		ImGui::End();
	}
}

void FileExplorerTab::OpenTab()
{
	myTabOpen = true;
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

void FileExplorerTab::UpdatePendingMove()
{
	if (myPendingMove.has_value())
	{
		const std::filesystem::path& source = myPendingMove->mySource;
		const std::filesystem::path& destination = myPendingMove->myDestination;
		std::filesystem::rename(source, destination);

		// Retain open state after move
		myNodeOpenMap[destination] = myNodeOpenMap[source];

		myNodeOpenMap.erase(source);
		myPendingMove.reset();
	}
}

void FileExplorerTab::DrawCustomDirectoryNode(const std::filesystem::path& aPath, const char* aLabel, float aMargin)
{
	bool& open = myNodeOpenMap[aPath];
	bool hasDirectories = HasDirectories(aPath);
	ImTextureID folderIcon = open ? (ImTextureID)myFolderIcon_Open : (ImTextureID)myFolderIcon_Closed;
	if (!hasDirectories)
		folderIcon = (ImTextureID)myFolderIcon_Empty;
	ImTextureID arrowIcon = open ? (ImTextureID)myArrowIcon_Down : (ImTextureID)myArrowIcon_Right;
	std::string buttonLabel = std::string(aLabel) + "_arrowButton";

	ImGui::PushID(aLabel);

	// Margin
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + aMargin * 16.0f);

	// Draw button
	if (hasDirectories)
	{
		ImVec4 transparent(0.0f, 0.0f, 0.0f, 0.0f);
		ImGui::PushStyleColor(ImGuiCol_Button, transparent);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, transparent);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, transparent);

		if (ImGui::ImageButton(buttonLabel.c_str(), arrowIcon, ImVec2(9,9))) 
			open = !open;

		ImGui::PopStyleColor(3);
		ImGui::SameLine();
	}
	else
	{
		// Create dummy for margin/padding if no button
		ImGui::Dummy(ImVec2(ImGui::GetFrameHeight() - 2, 0));
		ImGui::SameLine();
	}

	// Draw icon
	ImGui::Image(folderIcon, ImVec2(13, 13));
	ImGui::SameLine();

	// Node
	bool windowFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);
	bool selected = false;
	for (auto& path : mySelectedPaths)
		if (path == aPath)
		{
			selected = true;
			break;
		}

	ImVec4 transparent(0.0f, 0.0f, 0.0f, 0.0f);
	ImVec4 blue(0.26f, 0.59f, 0.98f, 0.31f);
	ImVec4 gray(0.5f, 0.5f, 0.5f, 0.25f);
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, selected ? (windowFocused ? blue : gray) : transparent);
	if (!windowFocused)
		ImGui::PushStyleColor(ImGuiCol_Header, gray);

	if (ImGui::Selectable(aLabel, selected, ImGuiSelectableFlags_SpanAllColumns))
		if (ImGui::GetIO().KeyCtrl)
			mySelectedPaths.insert(aPath);
		else
		{
			mySelectedPaths.clear();
			mySelectedPaths.insert(aPath);
		}

	if (!windowFocused)
		ImGui::PopStyleColor(2);
	else
		ImGui::PopStyleColor(1);

	// Drag Drop Logic
	DragDropDirectoryNode(aPath);

	ImGui::PopID();
}

bool FileExplorerTab::HasDirectories(const std::filesystem::path& aPath)
{
	for (const auto& entry : std::filesystem::directory_iterator(aPath))
		if (entry.is_directory())
			return true;

	return false;
}

void FileExplorerTab::DragDropDirectoryNode(const std::filesystem::path& aPath)
{
	// Drag Source
	if (ImGui::BeginDragDropSource())
	{
		ImGui::SetDragDropPayload(
			"DIRECTORY_NODE_MOVE",
			aPath.string().c_str(),
			aPath.string().size() + 1
		);

		ImGui::Text("%s", aPath.filename().string().c_str());

		ImGui::EndDragDropSource();
	}

	// Drag Target
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DIRECTORY_NODE_MOVE");

		if (payload)
		{
			std::filesystem::path source = static_cast<const char*>(payload->Data);
			std::filesystem::path destination = aPath / source.filename();

			if (!IsInsideDirectory(source, destination))
				myPendingMove.emplace(source, destination);
		}

		ImGui::EndDragDropTarget();
	}
}

bool FileExplorerTab::IsInsideDirectory(const std::filesystem::path& aSource, const std::filesystem::path& aDestination)
{
	std::filesystem::path sourceAbsolute = std::filesystem::weakly_canonical(aSource);
	std::filesystem::path destinationAbsolute = std::filesystem::weakly_canonical(aDestination);

	std::pair<std::filesystem::path::iterator, std::filesystem::path::iterator> mismatch =
		std::mismatch(sourceAbsolute.begin(), sourceAbsolute.end(), destinationAbsolute.begin(), destinationAbsolute.end());

	return mismatch.first == sourceAbsolute.end();
}

void FileExplorerTab::DrawDirectoryTree(const std::filesystem::path& aPath, int aMargin)
{
	std::string label = aPath.filename().string();
	bool selected = false;

	DrawCustomDirectoryNode(aPath, label.c_str(), aMargin);

	if (myNodeOpenMap[aPath])
		for (const auto& entry : std::filesystem::directory_iterator(aPath))
			if (entry.is_directory())
				DrawDirectoryTree(entry, aMargin + 1.8f);
}
