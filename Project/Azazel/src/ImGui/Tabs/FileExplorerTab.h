#pragma once
#include "ImGui/imguiIncludes.h"
#include "Assets/AssetGUID.h"
#include <unordered_map>
#include <unordered_set>
#include <filesystem>
#include <d3d11.h>
#include <wrl.h> 

using Microsoft::WRL::ComPtr;

struct PendingMove
{
	std::vector<std::filesystem::path> mySources;
	std::filesystem::path myDestination;
};

class FileExplorerTab
{
public:
	FileExplorerTab(std::unordered_map<std::string, ID3D11ShaderResourceView*>& aIcons);
	~FileExplorerTab() = default;

	void Update();
	void CheckInputs();
	void OpenTab();

private:
	void DrawLeftPanel();
	void DrawRightPanel();
	void PanelSplitter();
	void UpdatePendingMove();

	AssetGUID GetFolderGUID(const std::filesystem::path& aPath);
	void BuildGUIDCache();

	bool HasDirectories(const std::filesystem::path& aPath);
	bool IsInsideDirectory(const std::filesystem::path& aSource, const std::filesystem::path& aDestination);

	void BuildVisibleNodeList(const std::filesystem::path& aPath);
	void LeftClickDirectory(const std::filesystem::path& aPath);
	void ShiftSelectDirectory(const std::filesystem::path& aClickedPath);
	void DragDropDirectoryNode(const std::filesystem::path& aPath);
	void OpenParentDirectories(const std::filesystem::path& aPath);
	void NodeLogic(const std::filesystem::path& aPath, const std::string& aLabel);
	void StartRenaming(const std::filesystem::path& aPath);
	void Rename(const std::filesystem::path& aPath);
	std::string CheckValidName(const std::filesystem::path& aPath, const std::string& aName, bool aNewFile);
	void RightClickContext();
	void DrawDirectoryNode(const std::filesystem::path& aPath, float aMargin);
	void DrawDirectoryTree(const std::filesystem::path& aPath, int aMargin, int aMarginIncrement);

	bool myTabOpen = true;

	bool myLeftClickOnRelease = false;
	std::filesystem::path myRootPath = "Assets";
	std::filesystem::path myAnchorPath;
	std::vector<std::filesystem::path> myVisibleNodes;
	std::unordered_set<std::filesystem::path> mySelectedPaths;
	std::unordered_map<std::filesystem::path, AssetGUID> myFolderGUIDs;
	std::unordered_map<AssetGUID, bool> myNodeOpenMap;

	float myLeftPanelWidth = 250.0f;
	float myLeftPanelFocused = false;

	// Renaming
	char myRenameBuffer[256];
	bool myFocusRenameInputField = false;
	std::filesystem::path myRenamingPath;

	// Right-Click Context
	bool myRightClickContextOpen = false;
	bool myJustOpenedRightClickContext = false;
	ImVec2 myRightClickContextPos = { 0,0 };
	std::filesystem::path myRightClickedPath;
	std::optional<PendingMove> myPendingMove;

	// Icons
	ID3D11ShaderResourceView* myFolderIcon_Closed;
	ID3D11ShaderResourceView* myFolderIcon_Open;
	ID3D11ShaderResourceView* myFolderIcon_Empty;
	ID3D11ShaderResourceView* myArrowIcon_Right;
	ID3D11ShaderResourceView* myArrowIcon_Down;
};

