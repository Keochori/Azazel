#include "pch.h"
#include "Engine.h"
#include "Graphics/DX11.h"

#include "Tools/Input.h"
#include "Tools/Timer.h"

Engine::Engine(HWND& aHWND)
{
	myDX11Framework = std::make_unique<DX11>(aHWND);
}

Engine::~Engine()
{
}

void Engine::Update()
{
	myDX11Framework->BeginFrame();

	const float color[] = { 0.2f,0.2f,0.2f,1.0f };
	myDX11Framework->ClearBuffer(color);
	myDX11Framework->DrawCube(TIMER.GetTotalTime() - 5.0f, 1.0f, 0.0f, 6.0f);
	myDX11Framework->DrawGremlin(TIMER.GetTotalTime() - 5.0f, 0.0f, -1.5f, 4.0f);

	if (INPUT.IsKeyPressed(eKeys::G))
	{
		LOG("G");
	}

	myDX11Framework->EndFrame();
}
