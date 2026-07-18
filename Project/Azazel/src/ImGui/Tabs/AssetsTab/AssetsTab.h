#pragma once
#include "ImGui/imguiIncludes.h"
#include "DirectoryTree.h"
#include "Assets/AssetGUID.h"
#include <unordered_map>
#include <unordered_set>
#include <filesystem>
#include <d3d11.h>
#include <wrl.h> 

using Microsoft::WRL::ComPtr;

class AssetsTab
{
public:
	AssetsTab(std::unordered_map<std::string, ID3D11ShaderResourceView*>& aIcons);
	~AssetsTab() = default;

	void Shutdown();
	void Update();
	void OpenTab();

private:
	void LeftPanel();
	void PanelSplitter();

	DirectoryTree* myDirectoryTree;

	bool myTabOpen = true;
	float myLeftPanelWidth = 250.0f;
	bool myLeftPanelFocused = false;
};

