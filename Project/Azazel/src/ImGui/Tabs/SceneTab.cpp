#include "pch.h"
#include "SceneTab.h"

SceneTab::SceneTab(ComPtr<ID3D11ShaderResourceView>& aSceneSRV) : mySceneSRV(aSceneSRV) 
{
}

void SceneTab::Update()
{
	if (myTabOpen)
	{
		ImGui::Begin("Scene", &myTabOpen, ImGuiWindowFlags_NoCollapse);
		mySceneTabSize = ImGui::GetContentRegionAvail();
		ImGui::Image((ImTextureID)mySceneSRV.Get(), mySceneTabSize);
		ImGui::End();
	}
}

void SceneTab::OpenTab()
{
	myTabOpen = true;
}

const ImVec2& SceneTab::GetSceneTabSize()
{
	return mySceneTabSize;
}