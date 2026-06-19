#include "pch.h"
#include "FileExplorerTab.h"
#include <algorithm>
#include <fstream>

FileExplorerTab::FileExplorerTab(std::unordered_map<std::string, ID3D11ShaderResourceView*>& aIcons)
{
	mySelectedPaths.insert(myRootPath);
	myAnchorPath = myRootPath;
	BuildGUIDCache();

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
		DrawLeftPanel();
		// Panel Splitter
		PanelSplitter();
		// Right Panel (File Explorer)
		DrawRightPanel();

		// Check for Pending Move
		UpdatePendingMove();

		ImGui::End();
	}
}

void FileExplorerTab::OpenTab()
{
	myTabOpen = true;
}

void FileExplorerTab::DrawLeftPanel()
{
	ImGui::BeginChild("LeftPanel", ImVec2(myLeftPanelWidth, 0), true);

	myLeftPanelFocused = ImGui::IsWindowFocused();
	DrawDirectoryTree(myRootPath, 0, 1.8f);

	ImGui::EndChild();
}

void FileExplorerTab::DrawRightPanel()
{
	ImGui::BeginChild("RightPanel", ImVec2(0, 0), true);

	for (const auto& path : mySelectedPaths)
		ImGui::Text(path.string().c_str());
	ImGui::Text("AnchorPath: ");
	ImGui::SameLine();
	ImGui::Text(myAnchorPath.string().c_str());

	ImGui::EndChild();
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
		mySelectedPaths.clear();
		const std::vector<std::filesystem::path>& sources = myPendingMove->mySources;
		for (const auto& source : sources)
		{
			const std::filesystem::path destination = myPendingMove->myDestination / source.filename();

			// Move
			std::filesystem::rename(source, destination);

			// Retain selection
			mySelectedPaths.insert(destination);
		}

		// Rebuild GUID cache after moving files
		BuildGUIDCache();

		// Open destination folder after moving
		myNodeOpenMap[myFolderGUIDs[myPendingMove->myDestination]] = true;

		myPendingMove.reset();
	}
}

AssetGUID FileExplorerTab::GetFolderGUID(const std::filesystem::path& aPath)
{
	const std::filesystem::path metaPath = aPath / ".foldermeta";

	// Get GUID from file
	if (std::filesystem::exists(metaPath))
	{
		std::ifstream file(metaPath);

		AssetGUID guid;
		std::getline(file, guid);

		return guid;
	}

	// Create .foldermeta if not already existing
	AssetGUID guid = GenerateGUID();
	std::ofstream file(metaPath);
	file << guid;

	return guid;
}

void FileExplorerTab::BuildGUIDCache()
{
	myFolderGUIDs[myRootPath] = GetFolderGUID(myRootPath);

	for (const auto& entry : std::filesystem::recursive_directory_iterator(myRootPath))
		if (entry.is_directory())
			myFolderGUIDs[entry.path()] = GetFolderGUID(entry.path());
}

bool FileExplorerTab::HasDirectories(const std::filesystem::path& aPath)
{
	for (const auto& entry : std::filesystem::directory_iterator(aPath))
		if (entry.is_directory())
			return true;

	return false;
}

bool FileExplorerTab::IsInsideDirectory(const std::filesystem::path& aSource, const std::filesystem::path& aDestination)
{
	std::filesystem::path sourceAbsolute = std::filesystem::weakly_canonical(aSource);
	std::filesystem::path destinationAbsolute = std::filesystem::weakly_canonical(aDestination);

	std::pair<std::filesystem::path::iterator, std::filesystem::path::iterator> mismatch =
		std::mismatch(destinationAbsolute.begin(), destinationAbsolute.end(), sourceAbsolute.begin(), sourceAbsolute.end());

	return mismatch.first == destinationAbsolute.end();
}

void FileExplorerTab::BuildVisibleNodeList(const std::filesystem::path& aPath)
{
	myVisibleNodes.push_back(aPath);

	if (myNodeOpenMap[myFolderGUIDs[aPath]])
	{
		for (const auto& entry : std::filesystem::directory_iterator(aPath))
		{
			if (entry.is_directory())
				BuildVisibleNodeList(entry.path());
		}
	}
}

void FileExplorerTab::LeftClickDirectory(const std::filesystem::path& aPath)
{
	mySelectedPaths.clear();
	mySelectedPaths.insert(aPath);
	myAnchorPath = aPath;
	myLeftClickOnRelease = false;
}

void FileExplorerTab::ShiftSelectDirectory(const std::filesystem::path& aPath)
{
	auto anchorIt = std::find(myVisibleNodes.begin(), myVisibleNodes.end(), myAnchorPath);
	auto clickedIt = std::find(myVisibleNodes.begin(), myVisibleNodes.end(), aPath);

	if (anchorIt == myVisibleNodes.end() || clickedIt == myVisibleNodes.end())
		return;

	mySelectedPaths.clear();

	if (anchorIt > clickedIt)
		std::swap(anchorIt, clickedIt);

	for (auto it = anchorIt; it != std::next(clickedIt); it++)
		mySelectedPaths.insert(*it);
}

void FileExplorerTab::DragDropDirectoryNode(const std::filesystem::path& aPath)
{
	// Drag Source
	if (ImGui::BeginDragDropSource())
	{
		ImGui::SetDragDropPayload("DIRECTORY_NODE_MOVE",aPath.string().c_str(), aPath.string().size() + 1);
		ImGui::EndDragDropSource();
	}

	// Drag Target
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DIRECTORY_NODE_MOVE");

		if (payload)
		{
			std::filesystem::path source = static_cast<const char*>(payload->Data);
			std::filesystem::path destination = aPath;

			bool insideDirectory = false;
			for (const auto& path : mySelectedPaths)
			{
				if (IsInsideDirectory(destination, path))
				{
					insideDirectory = true;
					break;
				}
			}
			if (!insideDirectory)
			{
				std::vector<std::filesystem::path> myHighestDirectories;
				for (const auto& pathToCheck : mySelectedPaths)
				{
					bool highestDirectory = true;
					for (const auto& currentPath : mySelectedPaths)
					{
						if (pathToCheck != currentPath)
						{
							if (IsInsideDirectory(pathToCheck, currentPath))
							{
								highestDirectory = false;
								break;
							}
						}
					}
					if (highestDirectory)
						myHighestDirectories.push_back(pathToCheck);
				}
				myPendingMove.emplace(myHighestDirectories, destination);
			}
		}

		ImGui::EndDragDropTarget();
	}
}

void FileExplorerTab::OpenParentDirectories(const std::filesystem::path& aPath)
{
	if (aPath != myRootPath)
	{
		myNodeOpenMap[myFolderGUIDs[aPath.parent_path()]] = true;

		OpenParentDirectories(aPath.parent_path());
	}
}

void FileExplorerTab::NodeLogic(const std::filesystem::path& aPath, const std::string& aLabel)
{
	bool selected = false;
	for (const auto& path : mySelectedPaths)
	{
		if (path == aPath)
		{
			selected = true;
			break;
		}
	}

	ImVec4 transparent(0.0f, 0.0f, 0.0f, 0.0f);
	ImVec4 blue(0.26f, 0.59f, 0.98f, 0.31f);
	ImVec4 gray(0.5f, 0.5f, 0.5f, 0.25f);

	// Node color
	bool windowFocused = myLeftPanelFocused;
	ImGui::PushStyleColor(ImGuiCol_HeaderActive, blue);
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, selected ? (windowFocused ? blue : gray) : transparent);
	if (!windowFocused)
		ImGui::PushStyleColor(ImGuiCol_Header, gray);

	// Node
	if (ImGui::Selectable(aLabel.c_str(), selected, ImGuiSelectableFlags_SpanAllColumns))
	{
		if (myLeftClickOnRelease)
			LeftClickDirectory(aPath);
	}

	if (ImGui::IsItemClicked())
	{
		// Shift-Click
		if (ImGui::GetIO().KeyShift)
		{
			OpenParentDirectories(myAnchorPath);
			myVisibleNodes.clear();
			BuildVisibleNodeList(myRootPath);
			ShiftSelectDirectory(aPath);
		}
		// Ctrl-Click
		else if (ImGui::GetIO().KeyCtrl)
		{
			// Select/Deselect 
			if (mySelectedPaths.contains(aPath))
				mySelectedPaths.erase(aPath);
			else
				mySelectedPaths.insert(aPath);

			// Open all parent directories
			for (const auto& path : mySelectedPaths) 
				OpenParentDirectories(path);

			myAnchorPath = aPath;
		}
		// Left-Click
		else
		{
			if (!mySelectedPaths.contains(aPath))
				LeftClickDirectory(aPath);
			else
				myLeftClickOnRelease = true;
		}
	}

	// Pop ImGui style colors
	if (!windowFocused)
		ImGui::PopStyleColor(3);
	else
		ImGui::PopStyleColor(2);
}
 
void FileExplorerTab::DrawDirectoryNode(const std::filesystem::path& aPath, float aMargin)
{
	std::string label = aPath.filename().string();
	bool& open = myNodeOpenMap[myFolderGUIDs[aPath]];
	bool hasDirectories = HasDirectories(aPath);

	ImTextureID folderIcon = open ? (ImTextureID)myFolderIcon_Open : (ImTextureID)myFolderIcon_Closed;
	if (!hasDirectories)
		folderIcon = (ImTextureID)myFolderIcon_Empty;
	ImTextureID arrowIcon = open ? (ImTextureID)myArrowIcon_Down : (ImTextureID)myArrowIcon_Right;

	ImGui::PushID(label.c_str());

	// Margin
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + aMargin * 16.0f);

	// Draw open/close button
	if (hasDirectories)
	{
		ImVec4 transparent(0.0f, 0.0f, 0.0f, 0.0f);
		ImGui::PushStyleColor(ImGuiCol_Button, transparent);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, transparent);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, transparent);

		std::string buttonLabel = std::string(label) + "_arrowButton";
		if (ImGui::ImageButton(buttonLabel.c_str(), arrowIcon, ImVec2(9, 9)))
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

	// Draw folder icon
	ImGui::Image(folderIcon, ImVec2(13, 13));
	ImGui::SameLine();

	// Node
	NodeLogic(aPath, label);

	// Drag Drop Logic
	DragDropDirectoryNode(aPath);

	ImGui::PopID();
}

void FileExplorerTab::DrawDirectoryTree(const std::filesystem::path& aPath, int aMargin, int aMarginIncrement)
{
	DrawDirectoryNode(aPath, aMargin);

	if (myNodeOpenMap[myFolderGUIDs[aPath]])
		for (const auto& entry : std::filesystem::directory_iterator(aPath))
			if (entry.is_directory())
				DrawDirectoryTree(entry.path(), aMargin + aMarginIncrement, aMarginIncrement);
}
