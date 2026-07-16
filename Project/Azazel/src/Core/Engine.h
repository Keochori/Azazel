#pragma once
#include <memory>
#include "imgui/imgui.h"

class DX11;
class Renderer;
class ImGuiManager;
class AssetManager;
class Animator;
class Scene;

class Engine
{
public:
	Engine(HWND& aHWND, UINT aWindowWidth, UINT aWindowHeight);
	~Engine();

	void Shutdown();
	void Update();
	void OnWindowResize(UINT aClientWidth, UINT aClientHeight);
	void OnTextureResize(ImVec2 aSize);

	void SetWindowSize(UINT aWindowWidth, UINT aWindowHeight);
	void SetWindowMaximized(bool aMaximized);
	bool GetWindowMaximized();

private:
	void UpdateScene();
	void RenderScene();
	std::unique_ptr<DX11> myDX11;
	std::unique_ptr<Renderer> myRenderer;
	std::unique_ptr<ImGuiManager> myImGuiManager;
	std::unique_ptr<AssetManager> myAssetManager;
	std::unique_ptr<Animator> myAnimator;
	std::shared_ptr<Scene> myScene;

	bool myWindowMaximized = false;
	UINT myWindowWidth = 0u;
	UINT myWindowHeight = 0u;

	ImVec2 myCurrentSceneTabSize;
	bool myFullScreenMode = false;
};

