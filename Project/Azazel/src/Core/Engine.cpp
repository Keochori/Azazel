#include "pch.h"
#include "Engine.h"
#include "Graphics/DX11.h"
#include "Scene/Camera/Camera.h"
#include "Tools/Input.h"
#include "Tools/Timer.h"


Engine::Engine(HWND& aHWND)
{
	myDX11 = std::make_unique<DX11>(aHWND);
	myEditorCamera = std::make_unique<Camera>();

	myMaxCameraSpeed = 10;
	myMinCameraSpeed = 2;
	myCameraRotationSpeed = 2;
	myCurrentCameraSpeed = myMaxCameraSpeed * 0.5f;
}

Engine::~Engine()
{
}

void Engine::Update()
{
	UpdateCameraInput();

	// DX --------------------------
	myDX11->BeginFrame();

	const float color[] = { 0.2f,0.2f,0.2f,1.0f };
	myDX11->ClearBuffer(color);
	myDX11->DrawGremlin(TIMER.GetTotalTime() - 5.0f, 0.0f, -1.5f, 4.0f, myEditorCamera.get());

	myDX11->EndFrame();
	// -----------------------------
}

void Engine::UpdateCameraInput()
{
	float movementSpeedDelta = myCurrentCameraSpeed * TIMER.GetDeltaTime();

	// Change camera speed with mousewheel
	short wheelDelta = INPUT.GetMouseWheelDelta();
	if (wheelDelta != 0)
		myCurrentCameraSpeed = std::clamp(myCurrentCameraSpeed + wheelDelta * 0.01f, myMinCameraSpeed, myMaxCameraSpeed);

	// Holding right-click
	if (INPUT.IsMouseButtonDown(eKeys::MOUSERBUTTON))
	{
		// Movement
		if (INPUT.IsKeyDown(eKeys::W))
			myEditorCamera->Move(0, 0, movementSpeedDelta);
		if (INPUT.IsKeyDown(eKeys::S))
			myEditorCamera->Move(0, 0, -movementSpeedDelta);
		if (INPUT.IsKeyDown(eKeys::A))
			myEditorCamera->Move(-movementSpeedDelta, 0, 0);
		if (INPUT.IsKeyDown(eKeys::D))
			myEditorCamera->Move(movementSpeedDelta, 0, 0);
		if (INPUT.IsKeyDown(eKeys::Q))
			myEditorCamera->Move(0, -movementSpeedDelta, 0);
		if (INPUT.IsKeyDown(eKeys::E))
			myEditorCamera->Move(0, movementSpeedDelta, 0);

		// Rotation
		POINT mouseDelta = INPUT.GetMouseDelta();
		if (mouseDelta.x != 0 || mouseDelta.y != 0)
			myEditorCamera->Rotate(mouseDelta.y * myCameraRotationSpeed * 0.001f, mouseDelta.x * myCameraRotationSpeed * 0.001f);
	}
}
