#include "pch.h"
#include "Engine.h"
#include "Graphics/DX11.h"
#include "Graphics/Renderer.h"
#include "Assets/AssetManager.h"
#include "Scene/Object.h"
#include "Scene/Scene.h"
#include "Tools/Input.h"
#include "Tools/Timer.h"
#include "ImGui/ImGuiManager.h"

Engine::Engine(HWND& aHWND)
{
	myDX11 = std::make_unique<DX11>(aHWND);
	myRenderer = std::make_unique<Renderer>(myDX11->GetDevice(), myDX11->GetContext(), myDX11->GetScreenWidth() / myDX11->GetScreenHeight());
	myImGuiManager = std::make_unique<ImGuiManager>(aHWND, myDX11->GetDevice(), myDX11->GetContext());
	myAssetManager = std::make_unique<AssetManager>();
	myScene = std::make_shared<Scene>();

	// Create scene objects
	myScene->AddObject(CreateObject("gremlin"));
	myScene->AddObject(CreateObject("chest"));
	myScene->AddObject(CreateObject("cube"));
	myScene->AddObject(CreateObject("cube2"));
	myScene->AddObject(CreateObject("cube3"));
	myScene->AddObject(CreateObject("cube4"));
	myScene->AddObject(CreateObject("a"));

	// Fetch scene objects
	std::shared_ptr<Object> gremlin = myScene->GetObject("gremlin");
	std::shared_ptr<Object> chest = myScene->GetObject("chest");
	std::shared_ptr<Object> cube = myScene->GetObject("cube");
	std::shared_ptr<Object> cube2 = myScene->GetObject("cube2");
	std::shared_ptr<Object> cube3 = myScene->GetObject("cube3");
	std::shared_ptr<Object> cube4 = myScene->GetObject("cube4");

	// Set Mesh
	gremlin->SetMesh(myAssetManager->GetMesh("gremlin.fbx", myDX11->GetDevice()));
	chest->SetMesh(myAssetManager->GetMesh("chest.fbx", myDX11->GetDevice()));
	cube->SetMesh(myAssetManager->GetMesh("cube.fbx", myDX11->GetDevice()));
	cube2->SetMesh(myAssetManager->GetMesh("cube.fbx", myDX11->GetDevice()));
	cube3->SetMesh(myAssetManager->GetMesh("cube.fbx", myDX11->GetDevice()));
	cube4->SetMesh(myAssetManager->GetMesh("cube.fbx", myDX11->GetDevice()));

	// Create Material
	std::shared_ptr<Material> gremlinMaterial = myAssetManager->CreateMaterial("gremlin", myDX11->GetDevice());
	std::shared_ptr<Material> chestMaterial = myAssetManager->CreateMaterial("chest", myDX11->GetDevice());
	std::shared_ptr<Material> clothMaterial = myAssetManager->CreateMaterial("cloth", myDX11->GetDevice());
	std::shared_ptr<Material> noAlbedoMaterial = myAssetManager->CreateMaterial("noTexture", myDX11->GetDevice());
	std::shared_ptr<Material> noAlbedoMaterialOrange = myAssetManager->CreateMaterial("noTextureOrange", myDX11->GetDevice());
	gremlinMaterial->myAlbedoTexture = myAssetManager->GetTexture("gremlin.png", myDX11->GetDevice());
	chestMaterial->myAlbedoTexture = myAssetManager->GetTexture("chest.png", myDX11->GetDevice());
	clothMaterial->myAlbedoTexture = myAssetManager->GetTexture("cloth.png", myDX11->GetDevice());

	// Set Material
	gremlin->SetMaterial(gremlinMaterial);
	chest->SetMaterial(chestMaterial);
	cube->SetMaterial(clothMaterial);
	cube2->SetMaterial(noAlbedoMaterial);
	cube3->SetMaterial(noAlbedoMaterialOrange);
	cube3->GetMaterial()->SetAlbedoColor(217, 159, 35);
	// No cube4 material to showcase missing material color

	// Transform
	Transform& gremlinTransform = gremlin->GetTransform();
	gremlinTransform.SetPosition(gremlinTransform.myPosition.x, -1.5f, 4.0f);
	gremlinTransform.SetScale(0.05f, 0.05f, 0.05f);

	Transform& chestTransform = chest->GetTransform();
	chestTransform.SetPosition(-2.5f, -1.5f, 4.0f);
	chestTransform.SetScale(0.01f, 0.01f, 0.01f);

	Transform& cubeTransform = cube->GetTransform();
	cubeTransform.SetPosition(3.0f, -0.5f, 4.0f);

	Transform& cubeTransform2 = cube2->GetTransform();
	cubeTransform2.SetPosition(3.0f, -0.5f, 6.5f);

	Transform& cubeTransform3 = cube3->GetTransform();
	cubeTransform3.SetPosition(3.0f, -0.5f, 9.0f);

	Transform& cubeTransform4 = cube4->GetTransform();
	cubeTransform4.SetPosition(3.0f, -0.5f, 11.5f);
}

Engine::~Engine()
{
}

std::shared_ptr<Object> Engine::CreateObject(const std::string& aName)
{
	return std::make_shared<Object>(aName);
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
	myRenderer->Render(myDX11->GetContext(), myScene->GetEditorCameraViewMatrix(), myScene->GetObjects());
}