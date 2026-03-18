#pragma once
#include <memory>

class DX11;
class Camera;

class Engine
{
public:
	Engine(HWND& aHWND);
	~Engine();

	void Update();
	void UpdateCameraInput();

private:
	std::unique_ptr<DX11> myDX11;

	// Camera
	std::unique_ptr<Camera> myEditorCamera;
	float myCurrentCameraSpeed;
	float myMaxCameraSpeed;
	float myMinCameraSpeed;
	float myCameraRotationSpeed;
};

