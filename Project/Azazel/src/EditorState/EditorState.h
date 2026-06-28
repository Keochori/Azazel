#pragma once
#include "Assets/AssetGUID.h"
#include <unordered_set>
#include <filesystem>

class EditorState
{
public:
	EditorState(const EditorState&) = delete;
	const EditorState& operator=(const EditorState&) = delete;;

	static EditorState& GetInstance();

	void LoadState();
	void SaveState();

	void SetWindowMaximized(bool aMaximized);
	void SetWindowSize(UINT aWindowWidth, UINT aWindowHeight);
	void SetExpandedFolders(const std::unordered_set<AssetGUID>& aExpandedFolders);

	bool GetWindowMaximized();
	UINT GetWindowWidth();
	UINT GetWindowHeight();
	std::unordered_set<AssetGUID> GetExpandedFolders();
private:
	EditorState() = default;
	~EditorState() = default;

	const std::string myJsonPath = "ProjectSettings/EditorState.json";

	std::unordered_set<AssetGUID> myExpandedFolders;
	bool myWindowMaximized = false;
	UINT myWindowWidth = 0u;
	UINT myWindowHeight = 0u;
};

