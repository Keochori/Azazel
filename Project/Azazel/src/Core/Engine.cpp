#include "pch.h"
#include "Engine.h"
#include "Graphics/DX11.h"
#include "Graphics/Renderer.h"
#include "Assets/AssetManager.h"
#include "Scene/Object.h"
#include "Scene/Scene.h"
#include "Tools/Input.h"
#include "Tools/Timer.h"

Engine::Engine(HWND& aHWND)
{
	myDX11 = std::make_unique<DX11>(aHWND);
	myRenderer = std::make_unique<Renderer>(myDX11->GetDevice(), myDX11->GetContext());
	myAssetManager = std::make_unique<AssetManager>();
	myScene = std::make_shared<Scene>();

	myScene->AddObject(CreateObject("gremlin.fbx"));
	myScene->AddObject(CreateObject("chest.fbx"));
	myScene->AddObject(CreateObject("cube.fbx"));

	Transform& gremlinTransform = myScene->GetObjects()[0]->GetTransform();
	gremlinTransform.SetPosition(gremlinTransform.myPosition.x, -1.5f, 4.0f);
	gremlinTransform.SetRotation(gremlinTransform.myRotation.x, 180.0f, gremlinTransform.myRotation.z);
	gremlinTransform.SetScale(0.05f, 0.05f, 0.05f);

	Transform& chestTransform = myScene->GetObjects()[1]->GetTransform();
	chestTransform.SetPosition(-2.5f, -1.5f, 4.0f);
	chestTransform.SetRotation(chestTransform.myRotation.x, 180.0f, chestTransform.myRotation.z);
	chestTransform.SetScale(0.01f, 0.01f, 0.01f);

	Transform& cubeTransform = myScene->GetObjects()[2]->GetTransform();
	cubeTransform.SetPosition(3.0f, -0.5f, 4.0f);
}

Engine::~Engine()
{
}

std::shared_ptr<Object> Engine::CreateObject(std::string aMeshName)
{
	return std::make_shared<Object>(myAssetManager->GetMesh(aMeshName, myDX11->GetDevice()));
}

void Engine::Update()
{
	myDX11->BeginFrame();
	UpdateFrame();
	myDX11->EndFrame();
}

void Engine::UpdateFrame()
{
	const float color[] = { 0.2f,0.2f,0.2f,1.0f };
	myDX11->ClearBuffer(color);

	myScene->Update();
	myRenderer->Render(myDX11->GetContext(), myScene->GetEditorCameraViewMatrix(), myScene->GetObjects());
}
