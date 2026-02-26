#include "pch.h"
#include "Window.h"
#include "Tools/Input.h"
#include "Engine.h"

int main()
{
	LOG_SUCCESS("STARTED AZAZEL");

	Window window;
	Engine engine(window.GetHWND());

	bool running = true;
	while (running)
	{
		if (!window.ProcessMessages())
		{
			running = false;
			break;
		}

		// Main Loop
		engine.Update();
	}

	return 0;
}