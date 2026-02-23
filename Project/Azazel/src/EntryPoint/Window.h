#pragma once
#include <windows.h>

class Window
{
public:
    Window();
    ~Window();

    void CreateAndRegisterWindowClass(HINSTANCE& hInstance, LPCWSTR className);
    bool ProcessMessages();

    HWND& GetHWND();

private:
    HINSTANCE myHInstance;
    HWND myHWND;
};

