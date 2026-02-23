#include "pch.h"
#include "Window.h"
#include "Tools/Input.h"

int main()
{
	Window window;

	bool running = true;
	while (running)
	{
		if (!window.ProcessMessages())
		{
			running = false;
			break;
		}

		// Main Loop
		INPUT.Update();
	}

	return 0;
}