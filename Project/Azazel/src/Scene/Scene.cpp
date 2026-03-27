#include "pch.h"
#include "Scene.h"
#include "Object.h"
#include "Camera/EditorCamera.h"
#include "Tools/Timer.h"

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
	myObjects[0]->GetTransform().myRotation.y += 40.0f * TIMER.GetDeltaTime();
}

void Scene::AddObject(std::shared_ptr<Object> aObject)
{
	myObjects.emplace_back(aObject);
}

const std::shared_ptr<Object>& Scene::GetObject(const std::string& aName)
{
	for (std::shared_ptr<Object> obj : myObjects)
	{
		if (obj->GetName() == aName)
			return obj;
	}

	return nullptr;
}

const std::vector<std::shared_ptr<Object>>& Scene::GetObjects()
{
	return myObjects;
}

const XMMATRIX& Scene::GetEditorCameraViewMatrix()
{
	return myEditorCamera->GetViewMatrix();
}
