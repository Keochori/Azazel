#include "pch.h"
#include "Window.h"
#include "Tools/Input.h"
#include "Icon/resource.h"
#include "imgui/imgui.h"
#include "Core/Engine.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

Window::Window() : myHInstance(GetModuleHandle(nullptr))
{
	// Register the window class.
	LPCWSTR className = L"Azazel";
	CreateAndRegisterWindowClass(myHInstance, className);

	// Create the window.
	HWND hwnd = CreateWindowEx(
		0,                              // Optional window styles.
		className,						// Window class
		L"Azazel",						// Window text
		WS_OVERLAPPEDWINDOW,            // Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		NULL,							// Parent window    
		NULL,							// Menu
		myHInstance,					// Instance handle
		NULL							// Additional application data
	);

	if (hwnd == NULL)
	{
		LOG_WARNING("hwnd hasn't been initialized correctly.");
	}

	myHWND = hwnd;

	// Set window position and size
	int width = 1500;
	int height = 902;
	int posX = 190;
	int posY = 80;
	SetWindowPos(hwnd, nullptr, posX, posY, width, height, 0);
	ShowWindow(hwnd, SW_SHOW);
}

void Window::CreateAndRegisterWindowClass(HINSTANCE& aHInstance, LPCWSTR aClassName)
{
	LPCWSTR CLASS_NAME = aClassName;

	WNDCLASS wc = { };

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = aHInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.hIcon = LoadIcon(aHInstance, MAKEINTRESOURCE(IDI_ICON1)); // Load custom icon

	RegisterClass(&wc);
}

bool Window::ProcessMessages()
{
	MSG msg = { };

	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
			return false;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// Custom Updates
	UpdateMouseInsideWindow();

	return true;
}

void Window::UpdateMouseInsideWindow()
{
	POINT mousePos;
	if (!GetCursorPos(&mousePos))
		LOG_WARNING("GetCursorPos() ran unsuccessfully");

	RECT clientRect;
	if (!GetClientRect(myHWND, &clientRect))
		LOG_WARNING("GetClientRect() ran unsuccessfully");

	if (!ScreenToClient(myHWND, &mousePos))
		LOG_WARNING("ScreenToClient() ran unsuccessfully");

	bool inside = mousePos.x >= 0 && mousePos.y >= 0 &&
		mousePos.x < clientRect.right && mousePos.y < clientRect.bottom;

	INPUT.MouseInsideWindowUpdate(inside);
}

HWND& Window::GetHWND()
{
	return myHWND;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// ImGui
	extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
		return true;

	INPUT.UpdateEvents(uMsg, wParam, lParam);

	switch (uMsg)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		case WM_SIZE:
		{
			Engine* engine = reinterpret_cast<Engine*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
			if (engine)
			{
				UINT width = LOWORD(lParam);
				UINT height = HIWORD(lParam);
				engine->OnResize(width, height);
			}
		}
		return 0;

		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);

			FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 2));

			EndPaint(hwnd, &ps);
		}
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
