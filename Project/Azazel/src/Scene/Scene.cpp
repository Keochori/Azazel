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

Entity& Scene::CreateEntity()
{
	Entity newEntity(myRegistry);
	newEntity.AddComponent<TransformComponent>();
	myEntities.emplace_back(newEntity);
	return newEntity;
}

const std::vector<RenderData>& Scene::GetRenderData()
{
	myRenderData.clear();

	auto view = myRegistry.view<TransformComponent, MeshComponent>();
	for (auto entity : view)
	{
		TransformComponent& transformComponent = view.get<TransformComponent>(entity);
		MeshComponent& meshComponent = view.get<MeshComponent>(entity);

		myRenderData.emplace_back(transformComponent.myTransform, meshComponent.myMesh, meshComponent.myMaterial);
	}

	return myRenderData;
}

const XMMATRIX& Scene::GetEditorCameraViewMatrix()
{
	return myEditorCamera->GetViewMatrix();
}
