#pragma once
#include "ImGui/imguiIncludes.h"
#include <d3d11.h>
#include <wrl.h> 

using Microsoft::WRL::ComPtr;

class SceneTab
{
public:
	SceneTab(ComPtr<ID3D11ShaderResourceView>& aSceneSRV);
	~SceneTab() = default;

	void Update();
	void OpenTab();
	const ImVec2& GetSceneTabSize();

private:
	bool myTabOpen = true;

	ComPtr<ID3D11ShaderResourceView>& mySceneSRV; // Texture that the world gets rendered into
	ImVec2 mySceneTabSize;
};

