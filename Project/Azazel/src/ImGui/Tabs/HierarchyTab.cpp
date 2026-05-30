#include "pch.h"
#include "ImGui/imguiIncludes.h"
#include "HierarchyTab.h"
#include "Scene/Scene.h"
#include "Scene/Entity.h"

HierarchyTab::HierarchyTab(Scene* aScene) : myScene(aScene)
{
}

void HierarchyTab::Update()
{
	if (myTabOpen)
	{
		ImGui::Begin("Hierarchy", &myTabOpen, ImGuiWindowFlags_NoCollapse);

		bool anyItemHovered = false;

		for (Entity& entity : myScene->GetEntities())
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf;
			bool isSelected = (mySelectedEntity == &entity);

			if (isSelected)
				flags |= ImGuiTreeNodeFlags_Selected;

			if (ImGui::TreeNodeEx(entity.GetName().c_str(), flags))
				ImGui::TreePop();

			if (ImGui::IsItemClicked())
				mySelectedEntity = &entity;

			if (ImGui::IsItemHovered())
				anyItemHovered = true;
		}

		if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(0) && !anyItemHovered)
			mySelectedEntity = nullptr;

		ImGui::End();
	}
}

void HierarchyTab::OpenTab()
{
	myTabOpen = true;
}

Entity* HierarchyTab::GetSelectedEntity()
{
	return mySelectedEntity;
}


