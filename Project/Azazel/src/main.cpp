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
	int windowWidth = 1500;
	int windowHeight = 902;

	// Load State
	EditorState& editorState = EditorState::GetInstance();
	editorState.LoadState();
	int loadedWidth = editorState.GetWindowWidth();
	int loadedHeight = editorState.GetWindowHeight();
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
	WindowSize windowSize = GetWindowSize();
	Window window(windowSize.myWidth, windowSize.myHeight);
	Engine engine(window.GetHWND());
	SetWindowLongPtr(window.GetHWND(), GWLP_USERDATA, reinterpret_cast<LONG_PTR>(&engine));

	LOG_SUCCESS("STARTED AZAZEL");

	bool running = true;
	while (running)
	{
		if (!window.ProcessMessages())
		{
			// Engine shutdown
			engine.Shutdown();

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