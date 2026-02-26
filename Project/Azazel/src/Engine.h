#pragma once
#include <memory>

class DX11;

class Engine
{
public:
	Engine(HWND aHWND);
	~Engine();

	void Update();

private:
	std::unique_ptr<DX11> myDX11Framework;

	// Remove later
	float red = 0;
	float green = 0;
	float blue = 0;
};

