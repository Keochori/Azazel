#include "pch.h"
#include "Scene.h"
#include "Entity.h"
#include "Components/Components.h"
#include "Camera/EditorCamera.h"
#include "Graphics/RenderData.h"
#include "Animator/Animator.h"

Scene::Scene()
{
	float cameraStartingSpeed = 5;
	float cameraMinSpeed = 2;
	float cameraMaxSpeed = 10;
	float cameraRotationSpeed = 2;
	float cameraZoomSpeed = 6;
	myEditorCamera = std::make_unique<EditorCamera>
		(cameraStartingSpeed, cameraMinSpeed, cameraMaxSpeed, cameraRotationSpeed, cameraZoomSpeed);
}

Scene::~Scene()
{
}

void Scene::Update()
{
	myEditorCamera->Update();
}

Entity& Scene::CreateEntity(const std::string& aName)
{
	myEntities.emplace_back(myRegistry, aName);
	Entity& newEntity = myEntities.back();

	newEntity.AddComponent<TransformComponent>();
	return newEntity;
}

std::vector<Entity>& Scene::GetEntities()
{
	return myEntities;
}

entt::registry& Scene::GetRegistry()
{
	return myRegistry;
}

const std::vector<RenderData>& Scene::GetRenderData()
{
	myRenderDataList.clear();

	auto viewModels = myRegistry.view<TransformComponent, ModelComponent>(entt::exclude<AnimationComponent>);
	for (auto entity : viewModels)
	{
		TransformComponent& transformComponent = viewModels.get<TransformComponent>(entity);
		ModelComponent& modelComponent = viewModels.get<ModelComponent>(entity);

		myRenderDataList.emplace_back(transformComponent.myTransform, modelComponent.myModel, modelComponent.myMaterial, std::vector<DirectX::XMMATRIX>());
	}

	auto viewAnimatedModels = myRegistry.view<TransformComponent, ModelComponent, AnimationComponent>();
	for (auto entity : viewAnimatedModels)
	{
		TransformComponent& transformComponent = viewAnimatedModels.get<TransformComponent>(entity);
		ModelComponent& modelComponent = viewAnimatedModels.get<ModelComponent>(entity);
		AnimationComponent& animationComponent = viewAnimatedModels.get<AnimationComponent>(entity);

		std::vector<DirectX::XMMATRIX> finalBoneMatrices;
		if (animationComponent.myAnimation)
			finalBoneMatrices = animationComponent.myAnimation->myFinalBoneMatrices;
		myRenderDataList.emplace_back(transformComponent.myTransform, modelComponent.myModel, modelComponent.myMaterial, finalBoneMatrices);
	}

	return myRenderDataList;
}

const XMMATRIX& Scene::GetEditorCameraViewMatrix()
{
	return myEditorCamera->GetViewMatrix();
}
