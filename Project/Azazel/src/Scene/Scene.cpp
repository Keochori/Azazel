#include "pch.h"
#include "Scene.h"
#include "Entity.h"
#include "Components/Components.h"
#include "Camera/EditorCamera.h"
#include "Graphics/RenderData.h"

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
	myRenderData.clear();

	auto view = myRegistry.view<TransformComponent, ModelComponent>();
	for (auto entity : view)
	{
		TransformComponent& transformComponent = view.get<TransformComponent>(entity);
		ModelComponent& modelComponent = view.get<ModelComponent>(entity);

		myRenderData.emplace_back(transformComponent.myTransform, modelComponent.myModel, modelComponent.myMaterial);
	}

	return myRenderData;
}

const XMMATRIX& Scene::GetEditorCameraViewMatrix()
{
	return myEditorCamera->GetViewMatrix();
}
