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
	Engine(HWND& aHWND);
	~Engine();

	void Update();
	void OnWindowResize(UINT aWidth, UINT aHeight);
	void OnTextureResize(ImVec2 aSize);

private:
	void UpdateAndRenderGame();
	std::unique_ptr<DX11> myDX11;
	std::unique_ptr<Renderer> myRenderer;
	std::unique_ptr<ImGuiManager> myImGuiManager;
	std::unique_ptr<AssetManager> myAssetManager;
	std::unique_ptr<Animator> myAnimator;
	std::shared_ptr<Scene> myScene;

	ImVec2 myCurrentSceneTabSize;
	bool myFullScreenMode;
};

