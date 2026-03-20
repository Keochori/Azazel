#pragma once
#include <memory>

class DX11;
class Renderer;
class AssetManager;
class Object;
class Scene;

class Engine
{
public:
	Engine(HWND& aHWND);
	~Engine();

	std::shared_ptr<Object> CreateObject(std::string aMeshName);
	void Update();

private:
	void UpdateFrame();
	std::unique_ptr<DX11> myDX11;
	std::unique_ptr<Renderer> myRenderer;
	std::unique_ptr<AssetManager> myAssetManager;
	std::shared_ptr<Scene> myScene;
};

