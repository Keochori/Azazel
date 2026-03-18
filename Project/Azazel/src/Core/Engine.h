#pragma once
#include <memory>

class DX11;
class Scene;

class Engine
{
public:
	Engine(HWND& aHWND);
	~Engine();

	void Update();

private:
	void UpdateFrame();
	std::unique_ptr<DX11> myDX11;
	std::shared_ptr<Scene> myScene;
};

