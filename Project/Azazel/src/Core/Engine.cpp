#include "pch.h"
#include "Engine.h"
#include "Graphics/DX11.h"
#include "Graphics/Renderer.h"
#include "Assets/AssetManager.h"
#include "ImGui/ImGuiManager.h"
#include "Scene/Scene.h"

#include "Scene/Entity.h"
#include "Scene/Components/Components.h"

Engine::Engine(HWND& aHWND)
{
	myDX11 = std::make_unique<DX11>(aHWND);
	myRenderer = std::make_unique<Renderer>(myDX11->GetDevice(), myDX11->GetContext(), myDX11->GetScreenWidth() / myDX11->GetScreenHeight());
	myImGuiManager = std::make_unique<ImGuiManager>(aHWND, myDX11->GetDevice(), myDX11->GetContext());
	myAssetManager = std::make_unique<AssetManager>();
	myScene = std::make_shared<Scene>();

	// Create entities
	Entity gremlin = myScene->CreateEntity();
	Entity gremlin2 = myScene->CreateEntity();
	Entity chest = myScene->CreateEntity();
	Entity cube1 = myScene->CreateEntity();
	Entity cube2 = myScene->CreateEntity();
	Entity cube3 = myScene->CreateEntity();

	// Fetch and add components
	Transform& gremlinTransform = gremlin.GetComponent<TransformComponent>().myTransform;
	Transform& gremlin2Transform = gremlin2.GetComponent<TransformComponent>().myTransform;
	Transform& chestTransform = chest.GetComponent<TransformComponent>().myTransform;
	Transform& cube1Transform = cube1.GetComponent<TransformComponent>().myTransform;
	Transform& cube2Transform = cube2.GetComponent<TransformComponent>().myTransform;
	Transform& cube3Transform = cube3.GetComponent<TransformComponent>().myTransform;
	MeshComponent& gremlinMeshComponent = gremlin.AddComponent<MeshComponent>();
	MeshComponent& gremlin2MeshComponent = gremlin2.AddComponent<MeshComponent>();
	MeshComponent& chestMeshComponent = chest.AddComponent<MeshComponent>();
	MeshComponent& cube1MeshComponent = cube1.AddComponent<MeshComponent>();
	MeshComponent& cube2MeshComponent = cube2.AddComponent<MeshComponent>();
	MeshComponent& cube3MeshComponent = cube3.AddComponent<MeshComponent>();

	// Mesh
	gremlinMeshComponent.myMesh = myAssetManager->GetMesh("gremlin.fbx", myDX11->GetDevice());
	gremlin2MeshComponent.myMesh = myAssetManager->GetMesh("gremlin.fbx", myDX11->GetDevice());
	chestMeshComponent.myMesh = myAssetManager->GetMesh("chest.fbx", myDX11->GetDevice());
	cube1MeshComponent.myMesh = myAssetManager->GetMesh("cube.fbx", myDX11->GetDevice());
	cube2MeshComponent.myMesh = myAssetManager->GetMesh("cube.fbx", myDX11->GetDevice());
	cube3MeshComponent.myMesh = myAssetManager->GetMesh("cube.fbx", myDX11->GetDevice());

	// Material
	gremlinMeshComponent.myMaterial = myAssetManager->CreateMaterial("gremlin", myDX11->GetDevice());
	gremlinMeshComponent.myMaterial->myAlbedoTexture = myAssetManager->GetTexture("gremlin.png", myDX11->GetDevice());
	gremlin2MeshComponent.myMaterial = myAssetManager->CreateMaterial("gremlin2", myDX11->GetDevice());
	gremlin2MeshComponent.myMaterial->myAlbedoTexture = myAssetManager->GetTexture("gremlin.png", myDX11->GetDevice());
	gremlin2MeshComponent.myMaterial->myAlbedoColor = Color(0, 0, 255);
	chestMeshComponent.myMaterial = myAssetManager->CreateMaterial("chest", myDX11->GetDevice());
	chestMeshComponent.myMaterial->myAlbedoTexture = myAssetManager->GetTexture("chest.png", myDX11->GetDevice());
	cube1MeshComponent.myMaterial = myAssetManager->CreateMaterial("cube1", myDX11->GetDevice());
	cube1MeshComponent.myMaterial->myAlbedoTexture = myAssetManager->GetTexture("cloth.png", myDX11->GetDevice());
	cube2MeshComponent.myMaterial = myAssetManager->CreateMaterial("cube2", myDX11->GetDevice());

	// Transform
	gremlinTransform.myScale = { 0.05f,0.05f,0.05f };
	gremlinTransform.myPosition = { 0,-1.5f,4.0f};
	gremlin2Transform.myScale = { 0.05f,0.05f,0.05f };
	gremlin2Transform.myPosition = { 0,-1.0f,6.0f };
	chestTransform.myScale = { 0.01f,0.01f,0.01f };
	chestTransform.myPosition = { -2.5f, -1.5f, 4.0f };
	cube1Transform.myPosition = { 3.0f, -0.5f, 4.0f };
	cube2Transform.myPosition = { 3.0f, -0.5f, 6.5f };
	cube3Transform.myPosition = { 3.0f, -0.5f, 9.0f };
}

Engine::~Engine()
{
}

void Engine::Update()
{
	myImGuiManager->NewFrame();
	myImGuiManager->Update();
	myDX11->BeginFrame();
	UpdateFrame();
	myImGuiManager->Render();
	myDX11->EndFrame();
}

void Engine::OnResize(UINT aWidth, UINT aHeight)
{
	myDX11->OnResize(aWidth, aHeight);
	myRenderer->SetAspectRatio(myDX11->GetScreenWidth() / myDX11->GetScreenHeight());
}

void Engine::UpdateFrame()
{
	const float color[] = { 0.2f,0.2f,0.2f,1.0f };
	myDX11->ClearBuffer(color);

	myScene->Update();
	myRenderer->Render(myDX11->GetContext(), myScene->GetEditorCameraViewMatrix(), myScene->GetRenderData());
}