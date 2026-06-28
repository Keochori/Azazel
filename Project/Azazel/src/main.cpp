#include "pch.h"
#include "Core/Window.h"
#include "Tools/Input.h"
#include "Tools/Timer.h"
#include "Core/Engine.h"
#include "ImGui/imguiIncludes.h"
#include "EditorState/EditorState.h"

struct WindowSize
{
	int myWidth;
	int myHeight;
};

WindowSize GetWindowSize()
{
	// Default window-size
	int windowWidth = 1500;
	int windowHeight = 902;

	int loadedWidth = EditorState::GetInstance().GetWindowWidth();
	int loadedHeight = EditorState::GetInstance().GetWindowHeight();
	if (loadedWidth != 0 && loadedHeight != 0)
	{
		windowWidth = loadedWidth;
		windowHeight = loadedHeight;
	}

	return WindowSize(windowWidth, windowHeight);
}

int main()
{
	TIMER.Initialize();

	// Editor State
	EditorState::GetInstance().LoadState();
	WindowSize windowSize = GetWindowSize();
	bool maximizeState = EditorState::GetInstance().GetWindowMaximized();

	Window window(windowSize.myWidth, windowSize.myHeight, maximizeState);
	Engine engine(window.GetHWND(), windowSize.myWidth, windowSize.myHeight);
	SetWindowLongPtr(window.GetHWND(), GWLP_USERDATA, reinterpret_cast<LONG_PTR>(&engine));

	LOG_SUCCESS("STARTED AZAZEL");

	bool running = true;
	while (running)
	{
		if (!window.ProcessMessages())
		{
			// Engine shutdown
			engine.Shutdown();
			EditorState::GetInstance().SaveState();

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