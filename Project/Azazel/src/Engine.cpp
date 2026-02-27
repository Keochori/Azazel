#include "pch.h"
#include "Engine.h"
#include "Tools/Input.h"
#include "Graphics/DX11.h"

Engine::Engine(HWND& aHWND)
{
	myDX11Framework = std::make_unique<DX11>(aHWND);
}

Engine::~Engine()
{
}

void Engine::Update()
{
	float addValue = 0.0001f;
	red += addValue / 2;
	green += addValue * 5;
	blue += addValue / 6;
	myDX11Framework->ClearBuffer(red, green, blue);

	myDX11Framework->DrawTestTriangle();

	if (INPUT.IsKeyPressed(eKeys::G))
	{
		LOG("G");
	}

	myDX11Framework->EndFrame();
}
