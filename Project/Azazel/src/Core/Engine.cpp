#include "pch.h"
#include "Engine.h"
#include "Graphics/DX11.h"
#include "Scene/Scene.h"

#include "Tools/Input.h"
#include "Tools/Timer.h"

Engine::Engine(HWND& aHWND)
{
	myDX11 = std::make_unique<DX11>(aHWND);
	myScene = std::make_shared<Scene>();
}

Engine::~Engine()
{
}

void Engine::Update()
{
	myDX11->BeginFrame();
	UpdateFrame();
	myDX11->EndFrame();
}

void Engine::UpdateFrame()
{
	myScene->Update();

	const float color[] = { 0.2f,0.2f,0.2f,1.0f };
	myDX11->ClearBuffer(color);
	myDX11->DrawGremlin((float)TIMER.GetTotalTime() - 5.0f, 0.0f, -1.5f, 4.0f, myScene->GetEditorCameraViewMatrix());
}
