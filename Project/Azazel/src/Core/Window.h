#pragma once
#include <windows.h>

class Window
{
public:
    Window(int aWidth, int aHeight);
    ~Window() = default;

    bool ProcessMessages();
    HWND& GetHWND();

private:
    void CreateAndRegisterWindowClass(HINSTANCE& aHInstance, LPCWSTR aClassName);
    void UpdateMouseInsideWindow();

    HINSTANCE myHInstance;
    HWND myHWND;
};

