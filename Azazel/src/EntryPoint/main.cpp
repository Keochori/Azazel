#include "Window.h"

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
	}

	return 0;
}