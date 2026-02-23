#pragma once
#include <windows.h>

class Window
{
public:
    Window();
    ~Window();

    bool ProcessMessages();
    HWND& GetHWND();

private:
    void CreateAndRegisterWindowClass(HINSTANCE& hInstance, LPCWSTR className);
    void UpdateMouseInsideWindow();

    HINSTANCE myHInstance;
    HWND myHWND;
};

