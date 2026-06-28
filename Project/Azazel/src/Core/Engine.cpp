#include "pch.h"
#include "Engine.h"
#include "Graphics/DX11.h"
#include "Graphics/Renderer.h"
#include "Assets/AssetManager.h"
#include "ImGui/ImGuiManager.h"
#include "Scene/Scene.h"
#include "Tools/Input.h"
#include "Animator/Animator.h"
#include "Scene/Entity.h"
#include "Scene/Components/Components.h"
#include "EditorState/EditorState.h"

Engine::Engine(HWND& aHWND, UINT aWindowWidth, UINT aWindowHeight) : myWindowWidth(aWindowWidth), myWindowHeight(aWindowHeight)
{
	myDX11 = std::make_unique<DX11>(aHWND);
	myRenderer = std::make_unique<Renderer>(myDX11->GetDevice(), myDX11->GetContext(), myDX11->GetScreenWidth() / myDX11->GetScreenHeight());
	myAssetManager = std::make_unique<AssetManager>();
	myAnimator = std::make_unique<Animator>();
	myScene = std::make_shared<Scene>();

	// ImGui
	std::unordered_map<std::string, ID3D11ShaderResourceView*> icons;
	icons.emplace("folder_closed", myAssetManager->GetTexture("folder_closed.png", myDX11->GetDevice())->mySRV.Get());
	icons.emplace("folder_open", myAssetManager->GetTexture("folder_open.png", myDX11->GetDevice())->mySRV.Get());
	icons.emplace("folder_empty", myAssetManager->GetTexture("folder_empty.png", myDX11->GetDevice())->mySRV.Get());
	icons.emplace("arrow_right", myAssetManager->GetTexture("arrow_right.png", myDX11->GetDevice())->mySRV.Get());
	icons.emplace("arrow_down", myAssetManager->GetTexture("arrow_down.png", myDX11->GetDevice())->mySRV.Get());
	myImGuiManager = std::make_unique<ImGuiManager>(aHWND, myDX11->GetDevice(), myDX11->GetContext(), myDX11->GetTextureSRV(), icons, myScene.get());

	// Create entities
	Entity empty = myScene->CreateEntity("empty");
	Entity gremlin = myScene->CreateEntity("gremlin");
	Entity gremlin2 = myScene->CreateEntity("gremlin2");
	Entity chest = myScene->CreateEntity("chest");
	Entity cube1 = myScene->CreateEntity("cube1");
	Entity cube2 = myScene->CreateEntity("cube2");
	Entity cube3 = myScene->CreateEntity("cube3");
	Entity bot = myScene->CreateEntity("bot");

	// Fetch and add components
	Transform& gremlinTransform = gremlin.GetComponent<TransformComponent>().myTransform;
	Transform& gremlin2Transform = gremlin2.GetComponent<TransformComponent>().myTransform;
	Transform& chestTransform = chest.GetComponent<TransformComponent>().myTransform;
	Transform& cube1Transform = cube1.GetComponent<TransformComponent>().myTransform;
	Transform& cube2Transform = cube2.GetComponent<TransformComponent>().myTransform;
	Transform& cube3Transform = cube3.GetComponent<TransformComponent>().myTransform;
	Transform& botTransform = bot.GetComponent<TransformComponent>().myTransform;
	ModelComponent& gremlinModelComponent = gremlin.AddComponent<ModelComponent>();
	ModelComponent& gremlin2ModelComponent = gremlin2.AddComponent<ModelComponent>();
	ModelComponent& chestModelComponent = chest.AddComponent<ModelComponent>();
	ModelComponent& cube1ModelComponent = cube1.AddComponent<ModelComponent>();
	ModelComponent& cube2ModelComponent = cube2.AddComponent<ModelComponent>();
	ModelComponent& cube3ModelComponent = cube3.AddComponent<ModelComponent>();
	ModelComponent& botModelComponent = bot.AddComponent<ModelComponent>();
	AnimationComponent& gremlinAnimationComponent = gremlin.AddComponent<AnimationComponent>();
	AnimationComponent& gremlin2AnimationComponent = gremlin2.AddComponent<AnimationComponent>();
	AnimationComponent& botAnimationComponent = bot.AddComponent<AnimationComponent>();

	// Model
	gremlinModelComponent.myModel = myAssetManager->GetModel("gremlin.fbx", myDX11->GetDevice());
	gremlin2ModelComponent.myModel = myAssetManager->GetModel("gremlin.fbx", myDX11->GetDevice());
	chestModelComponent.myModel = myAssetManager->GetModel("chest.fbx", myDX11->GetDevice());
	cube1ModelComponent.myModel = myAssetManager->GetModel("cube.fbx", myDX11->GetDevice());
	cube2ModelComponent.myModel = myAssetManager->GetModel("cube.fbx", myDX11->GetDevice());
	cube3ModelComponent.myModel = myAssetManager->GetModel("cube.fbx", myDX11->GetDevice());
	botModelComponent.myModel = myAssetManager->GetModel("bot.fbx", myDX11->GetDevice());

	// Animation
	gremlinAnimationComponent.myAnimation = myAnimator->AddAnimation(gremlinModelComponent.myModel->mySkeleton, myAssetManager->GetAnimation("gremlin@run.fbx"));
	gremlin2AnimationComponent.myAnimation = myAnimator->AddAnimation(gremlin2ModelComponent.myModel->mySkeleton, myAssetManager->GetAnimation("gremlin@run.fbx"));
	gremlin2AnimationComponent.myAnimation->mySpeed = 0.2f;
	botAnimationComponent.myAnimation = myAnimator->AddAnimation(botModelComponent.myModel->mySkeleton, myAssetManager->GetAnimation("bot@idle.fbx"));

	// Material
	gremlinModelComponent.myMaterial = myAssetManager->CreateMaterial("gremlin", myDX11->GetDevice());
	gremlinModelComponent.myMaterial->myAlbedoTexture = myAssetManager->GetTexture("gremlin.png", myDX11->GetDevice());
	gremlin2ModelComponent.myMaterial = myAssetManager->CreateMaterial("gremlin2", myDX11->GetDevice());
	gremlin2ModelComponent.myMaterial->myAlbedoTexture = myAssetManager->GetTexture("gremlin.png", myDX11->GetDevice());
	gremlin2ModelComponent.myMaterial->myAlbedoColor = Color(0, 0, 255);
	chestModelComponent.myMaterial = myAssetManager->CreateMaterial("chest", myDX11->GetDevice());
	chestModelComponent.myMaterial->myAlbedoTexture = myAssetManager->GetTexture("chest.png", myDX11->GetDevice());
	cube1ModelComponent.myMaterial = myAssetManager->CreateMaterial("cube1", myDX11->GetDevice());
	cube1ModelComponent.myMaterial->myAlbedoTexture = myAssetManager->GetTexture("cloth.png", myDX11->GetDevice());
	cube2ModelComponent.myMaterial = myAssetManager->CreateMaterial("cube2", myDX11->GetDevice());
	botModelComponent.myMaterial = myAssetManager->CreateMaterial("bot", myDX11->GetDevice());

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
	botTransform.myPosition = { -2.5f, -1.5f, 8.0f };
	botTransform.myScale = { 0.025f,0.025f,0.025f };
}

Engine::~Engine() 
{
}

void Engine::Shutdown()
{
	myImGuiManager->Shutdown();
	EditorState::GetInstance().SetWindowSize(myWindowWidth, myWindowHeight);
	EditorState::GetInstance().SetWindowMaximized(myWindowMaximized);
}

void Engine::Update()
{
	// Fullscreen Mode button
	if (INPUT.IsKeyPressed(eKeys::F))
	{
		// Dont change screen mode if window size is too small
		if (myDX11->GetScreenWidth() < 1.0f || myDX11->GetScreenHeight() < 1.0f)
			return;

		myFullScreenMode = !myFullScreenMode;
		if (myFullScreenMode)
		{
			OnWindowResize(myDX11->GetScreenWidth(), myDX11->GetScreenHeight());
			myCurrentSceneTabSize = ImVec2(); // Forces OnTextureResize in update-loop after exiting fullscreen mode
		}
		else
		{
			OnTextureResize(ImVec2(myDX11->GetScreenWidth(), myDX11->GetScreenHeight())); // Makes sure texture and DSV are the same size again
		}
	}

	// Rendering & DX11
	if (myFullScreenMode)
	{
		myDX11->ClearRTV(true);
		myDX11->BindRTV(true);
		UpdateAndRenderGame();
		myDX11->PresentFrame();
	}
	else
	{
		// Render Scene to Texture2D
		myDX11->SetViewPort(myCurrentSceneTabSize.x, myCurrentSceneTabSize.y);
		myDX11->ClearTextureRTV();
		myDX11->BindTextureRTV();
		UpdateAndRenderGame();

		// Now render ImGui to window
		myDX11->SetViewPort(myDX11->GetScreenWidth(), myDX11->GetScreenHeight());
		myDX11->ClearRTV();
		myDX11->BindRTV();
		myImGuiManager->NewFrame();
		myImGuiManager->Update();
		myImGuiManager->Render();
		myDX11->PresentFrame();

		// Check if ImGui scene tab was resized
		ImVec2 sceneTabSize = myImGuiManager->GetSceneTabSize();
		if (myCurrentSceneTabSize != sceneTabSize)
			OnTextureResize(sceneTabSize);
	}
}

void Engine::OnWindowResize(UINT aClientWidth, UINT aClientHeight)
{
	if (aClientWidth <= 0 || aClientHeight <= 0)
		return;

	myDX11->OnWindowResize(aClientWidth, aClientHeight, myFullScreenMode);
	if (myFullScreenMode)
		myRenderer->SetAspectRatio(myDX11->GetScreenWidth() / myDX11->GetScreenHeight());
}

void Engine::OnTextureResize(ImVec2 aSize)
{
	if (aSize.x <= 0 || aSize.y <= 0)
		return;

	myDX11->OnTextureResize(aSize.x, aSize.y);
	if (!myFullScreenMode)
		myRenderer->SetAspectRatio(aSize.x / aSize.y);
	myCurrentSceneTabSize = aSize;
}

void Engine::SetWindowMaximized(bool aMaximized)
{
	myWindowMaximized = aMaximized;
}

bool Engine::GetWindowMaximized()
{
	return myWindowMaximized;
}

void Engine::SetWindowSize(UINT aWindowWidth, UINT aWindowHeight)
{
	myWindowWidth = aWindowWidth;
	myWindowHeight = aWindowHeight;
}

void Engine::UpdateAndRenderGame()
{
	myScene->Update();
	myAnimator->Update();
	myRenderer->Render(myDX11->GetContext(), myScene->GetEditorCameraViewMatrix(), myScene->GetRenderData());
}