#include "pch.h"
#include "Window.h"
#include "Tools/Input.h"
#include "Tools/Timer.h"
#include "Core/Engine.h"

int main()
{
	TIMER.Initialize();

	Window window;
	Engine engine(window.GetHWND());

	LOG_SUCCESS("STARTED AZAZEL");

	bool running = true;
	while (running)
	{
		if (!window.ProcessMessages())
		{
			running = false;
			break;
		}

		// Main Loop
		INPUT.UpdateStates();
		TIMER.Update();
		engine.Update();
	}

	return 0;
}