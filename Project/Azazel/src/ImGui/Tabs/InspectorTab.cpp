#include "pch.h"
#include "InspectorTab.h"
#include "Scene/Scene.h"
#include "Scene/Entity.h"
#include "Scene/Components/Components.h"

InspectorTab::InspectorTab(Scene* aScene) : myScene(aScene)
{
	// Register components to draw
	RegisterComponentToDraw<TransformComponent>("Transform");
	RegisterComponentToDraw<ModelComponent>("Model");
}

void InspectorTab::Update(Entity* aEntity)
{
	if (myTabOpen)
	{
		mySelectedEntity = aEntity;
		ImGui::Begin("Inspector", &myTabOpen, ImGuiWindowFlags_NoCollapse | ImGuiTabBarFlags_None);

		if (mySelectedEntity != nullptr)
			for (auto& drawFn : myComponentsToDraw)
				drawFn(myScene->GetRegistry(), mySelectedEntity->GetHandle());

		ImGui::End();
	}
}

void InspectorTab::OpenTab()
{
	myTabOpen = true;
}

void InspectorTab::DrawComponentUI(TransformComponent& aTransformComponent)
{
	ImGui::DragFloat3("Position", &aTransformComponent.myTransform.myPosition.x, 0.1f);
}

void InspectorTab::DrawComponentUI(ModelComponent& aModelComponent)
{

}