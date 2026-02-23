#include "pch.h"
#include "Window.h"
#include "Tools/Input.h"

int main()
{
	Window window;

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
	}

	return 0;
}