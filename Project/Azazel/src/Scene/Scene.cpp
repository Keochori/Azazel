#include "pch.h"
#include "Scene.h"
#include "Camera/EditorCamera.h"

Scene::Scene()
{
	myEditorCamera = std::make_unique<EditorCamera>(5, 2, 10, 2, 6);
}

Scene::~Scene()
{
}

void Scene::Update()
{
	myEditorCamera->Update();
}

XMMATRIX Scene::GetEditorCameraViewMatrix() const
{
	return myEditorCamera->GetViewMatrix();
}
