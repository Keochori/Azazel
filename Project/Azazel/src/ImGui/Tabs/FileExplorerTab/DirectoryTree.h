#pragma once
#include "ImGui/imguiIncludes.h"
#include "Assets/AssetGUID.h"
#include <filesystem>
#include <unordered_set>
#include <unordered_map>
#include <d3d11.h>
#include <wrl.h> 

struct PendingMove
{
	std::vector<std::filesystem::path> mySources;
	std::filesystem::path myDestination;
};

class DirectoryTree
{
public:
	DirectoryTree(bool& aLeftPanelFocused, std::unordered_map<std::string, ID3D11ShaderResourceView*>& aIcons);
	~DirectoryTree() = default;

	void Draw();
	void Shutdown();
	void Update();
	void CheckInputs();
	void UpdatePendingMove();

private:
	void BuildVisibleNodeList(const std::filesystem::path& aPath);
	AssetGUID GetDirectoryGUID(const std::filesystem::path& aPath);
	void RebuildGUIDCache();

	bool HasDirectories(const std::filesystem::path& aPath);
	bool IsInsideDirectory(const std::filesystem::path& aSource, const std::filesystem::path& aDestination);

	void LeftClick(const std::filesystem::path& aClickedPath);
	void ShiftSelect(const std::filesystem::path& aClickedPath);
	void DragDrop(const std::filesystem::path& aPath);

	void OpenParentDirectories(const std::filesystem::path& aPath);
	void ActivateRenaming(const std::filesystem::path& aPath);
	void Rename(const std::filesystem::path& aPath);
	std::string CheckValidName(const std::filesystem::path& aPath, const std::string& aName, bool aNewFile);

	std::vector<std::filesystem::path> GetHighestDirectories(const std::unordered_set<std::filesystem::path>& aDirectories);

	void RightClickContext();
	void DeleteConfirmContext();

	void NodeLogic(const std::filesystem::path& aPath, const std::string& aLabel);
	void DrawDirectoryNode(const std::filesystem::path& aPath, float aMargin);
	void DrawDirectoryTree(const std::filesystem::path& aPath, int aMargin, int aMarginIncrement);

	bool& myLeftPanelFocused;
	bool myLeftClickOnRelease = false;
	std::filesystem::path myRootPath = "Assets";
	std::filesystem::path myAnchorPath;
	std::vector<std::filesystem::path> myVisibleNodes;
	std::unordered_set<std::filesystem::path> mySelectedDirectories;
	std::unordered_map<std::filesystem::path, AssetGUID> myDirectoryGUIDs;
	std::unordered_map<AssetGUID, bool> myExpandedDirectoriesMap;

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

	// Delete Confirm Context
	bool myDeleteConfirmContextOpen = false;

	// Icons
	ID3D11ShaderResourceView* myFolderIcon_Closed;
	ID3D11ShaderResourceView* myFolderIcon_Open;
	ID3D11ShaderResourceView* myFolderIcon_Empty;
	ID3D11ShaderResourceView* myArrowIcon_Right;
	ID3D11ShaderResourceView* myArrowIcon_Down;
	ID3D11ShaderResourceView* myFileIcon;
};

