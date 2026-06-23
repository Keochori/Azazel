#include "pch.h"
#include "FileExplorerTab.h"
#include <algorithm>
#include <fstream>
#include <cctype>

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

		// Check for keyboard inputs
		CheckInputs();

		ImGui::End();
	}
}

void FileExplorerTab::CheckInputs()
{
	// Renaming
	if (ImGui::IsKeyPressed(ImGuiKey_F2))
		if (!myRightClickContextOpen)
			StartRenaming(myAnchorPath);
	if (ImGui::IsKeyPressed(ImGuiKey_Escape))
		if (!myRenamingPath.empty())
			myRenamingPath.clear();

	// Right-Click Context
	if (ImGui::IsKeyPressed(ImGuiKey_Escape))
		if (myRightClickContextOpen)
			myRightClickContextOpen = false;
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
	RightClickContext();

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
		bool foundAnchor = false;
		mySelectedPaths.clear();
		const std::vector<std::filesystem::path>& sources = myPendingMove->mySources;
		for (const auto& source : sources)
		{
			const std::filesystem::path destination = myPendingMove->myDestination / source.filename();

			// Retain anchor path
			if (source == myAnchorPath)
			{
				myAnchorPath = destination;
				foundAnchor = true;
			}

			// Move
			std::filesystem::rename(source, destination);

			// Retain selection
			mySelectedPaths.insert(destination);
		}

		// Assign new anchor if none was found
		if (!foundAnchor)
			myAnchorPath = *mySelectedPaths.begin();

		// Rebuild GUID cache after moving files
		BuildGUIDCache();

		// Open destination folder after moving
		myNodeOpenMap[myFolderGUIDs[myPendingMove->myDestination]] = true;

		myLeftClickOnRelease = false;
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
	myFolderGUIDs.clear();
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
	bool nodeActivated = false;
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
	bool focused = myLeftPanelFocused || myRightClickContextOpen;
	ImGui::PushStyleColor(ImGuiCol_HeaderActive, blue);
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, selected ? (focused ? blue : gray) : transparent);
	if (!focused)
		ImGui::PushStyleColor(ImGuiCol_Header, gray);

	// Rename Input Field
	if (myRenamingPath == aPath)
	{
		bool renamed = false;
		if (myFocusRenameInputField)
		{
			ImGui::SetKeyboardFocusHere();
			myFocusRenameInputField = false;
		}
		if (ImGui::InputText("##Rename", myRenameBuffer, sizeof(myRenameBuffer), 
			ImGuiInputTextFlags_AutoSelectAll | 
			ImGuiInputTextFlags_EnterReturnsTrue))
		{
			myRenamingPath.clear();
			Rename(aPath);
			renamed = true;
		}

		if (ImGui::IsItemDeactivated())
		{
			if (!renamed)
			{
				myRenamingPath.clear();
				Rename(aPath);
			}
		}
	}
	// Node
	else
	{
		if (ImGui::Selectable(aLabel.c_str(), selected, ImGuiSelectableFlags_SpanAllColumns))
		{
			if (myLeftClickOnRelease)
				LeftClickDirectory(aPath);
		}

		nodeActivated = ImGui::IsItemActivated();
	}

	// Left-Click
	bool clicked = ImGui::IsItemClicked(ImGuiMouseButton_Left);
	if (nodeActivated)
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
			if (!mySelectedPaths.contains(aPath) || !clicked)
				LeftClickDirectory(aPath);
			else
				myLeftClickOnRelease = true;
		}
	}

	// Right-Click
	if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
	{
		if (!mySelectedPaths.contains(aPath))
			LeftClickDirectory(aPath);
		
		myRightClickContextPos = ImGui::GetMousePos();
		myJustOpenedRightClickContext = true;
		myRightClickContextOpen = true;
		myRightClickedPath = aPath;
	}

	// Pop ImGui style colors
	if (!focused)
		ImGui::PopStyleColor(3);
	else
		ImGui::PopStyleColor(2);
}

void FileExplorerTab::StartRenaming(const std::filesystem::path& aPath)
{
	if (mySelectedPaths.size() == 1 && myAnchorPath != myRootPath)
	{
		myFocusRenameInputField = true;
		myRenamingPath = aPath;
		strcpy_s(myRenameBuffer, myRenamingPath.filename().string().c_str());
	}
}

void FileExplorerTab::Rename(const std::filesystem::path& aPath)
{
	std::string renameString(myRenameBuffer);

	// Renomve invalid characters
	while (!renameString.empty() && (renameString.back() == ' ' || renameString.back() == '.'))
		renameString.pop_back();

	if (!renameString.empty())
	{
		const std::filesystem::path newPath = aPath.parent_path() / CheckValidName(aPath, renameString, false);

		// Rename
		std::filesystem::rename(aPath, newPath);

		// Rebuild GUID Cache
		BuildGUIDCache();

		// Re-select node
		if (mySelectedPaths.contains(aPath))
		{
			mySelectedPaths.erase(aPath);
			mySelectedPaths.insert(newPath);
			if (myAnchorPath == aPath)
				myAnchorPath = newPath;
		}
	}
}

std::string FileExplorerTab::CheckValidName(const std::filesystem::path& aPath, const std::string& aName, bool aNewFile)
{
	std::string currentName = aName;

	std::filesystem::path pathToIterate = aPath;
	if (!aNewFile)
		pathToIterate = aPath.parent_path();

	bool foundValidName = false;
	while (!foundValidName)
	{
		foundValidName = true;
		for (const auto& entry : std::filesystem::directory_iterator(pathToIterate))
		{
			if (entry.is_directory())
			{
				if (entry.path() != aPath)
				{
					std::string fileName = entry.path().filename().string();
					if (fileName == currentName)
					{
						foundValidName = false;

						// If number present at the end of filename, iterate it by one.
						char lastChar = fileName.back();
						if (std::isdigit(lastChar))
						{
							bool allDigits = true;
							for (int i = 1; i < fileName.size(); i++)
							{
								if (!std::isdigit(fileName[fileName.size() - i]))
								{
									int digit = std::stoi(fileName.substr(fileName.size() - (i - 1)));
									currentName = aName + " " + std::to_string((digit + 1));
									allDigits = false;
									break;
								}
							}

							if (allDigits)
							{
								int digit = std::stoi(fileName);
								currentName = std::to_string((digit + 1));
							}
						}
						// Add '1' to filename if no present numbers
						else
						{
							currentName += " 1";
						}
					}
				}
			}
		}
	}

	return currentName;
}

void FileExplorerTab::RightClickContext()
{
	bool contextHovered = false;

	if (myRightClickContextOpen)
	{
		ImGui::SetNextWindowPos(myRightClickContextPos);

		ImGui::Begin("RightClickContext",
			nullptr,
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_AlwaysAutoResize);

		// Menu Items
		if (ImGui::MenuItem("Create Folder"))
		{
			myRightClickContextOpen = false;

			mySelectedPaths.clear();
			mySelectedPaths.insert(myAnchorPath);

			// Open parent node
			myNodeOpenMap[myFolderGUIDs[myRightClickedPath]] = true;

			// Create
			std::filesystem::path newDirectory = myRightClickedPath / CheckValidName(myRightClickedPath, "New File", true);
			std::filesystem::create_directory(newDirectory);

			// Create new GUID and add to cache
			myFolderGUIDs[newDirectory] = GetFolderGUID(newDirectory);
			StartRenaming(newDirectory);
		}

		if (ImGui::MenuItem("Delete"))
		{
			myRightClickContextOpen = false;
		}

		bool renameEnabled = (mySelectedPaths.size() == 1 && myAnchorPath != myRootPath);
		if (ImGui::MenuItem("Rename", nullptr, false, renameEnabled))
		{
			myRightClickContextOpen = false;
			StartRenaming(myRightClickedPath);
		}

		contextHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows);
		ImGui::End();

		if (myJustOpenedRightClickContext)
			myJustOpenedRightClickContext = false;
		else if (myRightClickContextOpen && !contextHovered && (ImGui::IsMouseClicked(ImGuiMouseButton_Left) || ImGui::IsMouseClicked(ImGuiMouseButton_Right)))
			myRightClickContextOpen = false;
	}
}
 
void FileExplorerTab::DrawDirectoryNode(const std::filesystem::path& aPath, float aMargin)
{
	bool& open = myNodeOpenMap[myFolderGUIDs[aPath]];
	bool hasDirectories = HasDirectories(aPath);

	ImTextureID folderIcon = open ? (ImTextureID)myFolderIcon_Open : (ImTextureID)myFolderIcon_Closed;
	if (!hasDirectories)
		folderIcon = (ImTextureID)myFolderIcon_Empty;
	ImTextureID arrowIcon = open ? (ImTextureID)myArrowIcon_Down : (ImTextureID)myArrowIcon_Right;

	ImGui::PushID(aPath.string().c_str());

	// Margin
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + aMargin * 16.0f);

	// Draw open/close button
	if (hasDirectories)
	{
		ImVec4 transparent(0.0f, 0.0f, 0.0f, 0.0f);
		ImGui::PushStyleColor(ImGuiCol_Button, transparent);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, transparent);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, transparent);

		std::string buttonLabel = std::string(aPath.string()) + "_arrowButton";
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
	NodeLogic(aPath, aPath.filename().string());

	// Drag Drop Logic
	DragDropDirectoryNode(aPath);

	ImGui::PopID();
}

void FileExplorerTab::DrawDirectoryTree(const std::filesystem::path& aPath, int aMargin, int aMarginIncrement)
{
	DrawDirectoryNode(aPath, aMargin);

	if (myFolderGUIDs.contains(aPath))
		if (myNodeOpenMap[myFolderGUIDs[aPath]])
			for (const auto& entry : std::filesystem::directory_iterator(aPath))
				if (entry.is_directory())
					DrawDirectoryTree(entry.path(), aMargin + aMarginIncrement, aMarginIncrement);
}
