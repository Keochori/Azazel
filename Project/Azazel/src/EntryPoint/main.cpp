#include "pch.h"
#include "Window.h"
#include "Tools/Input.h"
#include "Graphics/Graphics.h"

int main()
{
	Window window;
	Graphics graphics(window.GetHWND());

	LOG_SUCCESS("STARTED AZAZEL");

	float red = 0;
	float green = 0;
	float blue = 0;

	bool running = true;
	while (running)
	{
		if (!window.ProcessMessages())
		{
			running = false;
			break;
		}

		// Main Loop
		graphics.EndFrame();
		
		float addValue = 0.0001f;
		red += addValue /2;
		green += addValue *5;
		blue += addValue / 6;
		graphics.ClearBuffer(red, green, blue);

		if (INPUT.IsKeyPressed(eKeys::G))
		{
			LOG("G");
		}
	}

	return 0;
}