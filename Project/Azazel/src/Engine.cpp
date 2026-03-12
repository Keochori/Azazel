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

	myDX11Framework->ClearBuffer(0.2f, 0.2f, 0.2f);
	myDX11Framework->DrawCube(TIMER.GetTotalTime() - 5.0f, 0.0f, 0.0f, 5.0f);
	myDX11Framework->DrawCube(TIMER.GetTotalTime(), 2.0f,0.5f, 5.0f);

	if (INPUT.IsKeyPressed(eKeys::G))
	{
		LOG("G");
	}

	myDX11Framework->EndFrame();
}
