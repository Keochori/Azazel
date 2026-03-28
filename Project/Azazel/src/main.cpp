#include "pch.h"
#include "Core/Window.h"
#include "Tools/Input.h"
#include "Tools/Timer.h"
#include "Core/Engine.h"
#include "ImGui/imguiIncludes.h"

int main()
{
	TIMER.Initialize();

	Window window;
	Engine engine(window.GetHWND());
	SetWindowLongPtr(window.GetHWND(), GWLP_USERDATA, reinterpret_cast<LONG_PTR>(&engine));

	LOG_SUCCESS("STARTED AZAZEL");

	bool running = true;
	while (running)
	{
		if (!window.ProcessMessages())
		{
			// ImGui shutdown
			ImGui_ImplDX11_Shutdown();
			ImGui_ImplWin32_Shutdown();
			ImGui::DestroyContext();

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