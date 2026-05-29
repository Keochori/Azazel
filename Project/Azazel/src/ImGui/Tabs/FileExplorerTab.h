#pragma once
#include "ImGui/imguiIncludes.h"
#include <unordered_map>
#include <unordered_set>
#include <filesystem>
#include <d3d11.h>
#include <wrl.h> 

using Microsoft::WRL::ComPtr;

struct PendingMove
{
	std::filesystem::path mySource;
	std::filesystem::path myDestination;
};

class FileExplorerTab
{
public:
	FileExplorerTab(std::unordered_map<std::string, ID3D11ShaderResourceView*>& aIcons);
	~FileExplorerTab() = default;

	void Update();
	void OpenTab();

private:
	void PanelSplitter();
	void UpdatePendingMove();
	void DrawCustomDirectoryNode(const std::filesystem::path& aPath, const char* aLabel, float aMargin);
	bool HasDirectories(const std::filesystem::path& aPath);
	void DragDropDirectoryNode(const std::filesystem::path& aPath);
	bool IsInsideDirectory(const std::filesystem::path& aSource, const std::filesystem::path& aDestination);
	void DrawDirectoryTree(const std::filesystem::path& aPath, int aMargin);

	bool myTabOpen = true;
	std::filesystem::path myAssetsPath = "Assets";
	std::unordered_set<std::filesystem::path> mySelectedPaths;
	std::unordered_map<std::filesystem::path, bool> myNodeOpenMap;
	std::optional<PendingMove> myPendingMove;

	float myLeftPanelWidth = 250.0f;
	ID3D11ShaderResourceView* myFolderIcon_Closed;
	ID3D11ShaderResourceView* myFolderIcon_Open;
	ID3D11ShaderResourceView* myFolderIcon_Empty;
	ID3D11ShaderResourceView* myArrowIcon_Right;
	ID3D11ShaderResourceView* myArrowIcon_Down;
};

