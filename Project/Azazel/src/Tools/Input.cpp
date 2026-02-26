#include "pch.h"
#include "Input.h"
#include <windowsx.h>

Input Input::ourInstance;

Input& Input::GetInstance()
{
	return ourInstance;
}

bool Input::IsKeyDown(eKeys aKeyCode) const
{
	return myCurrentState[(int)aKeyCode] && myPreviousState[(int)aKeyCode];
}

bool Input::IsKeyPressed(eKeys aKeyCode) const
{
	return myCurrentState[(int)aKeyCode] && !myPreviousState[(int)aKeyCode];
}

bool Input::IsKeyUp(eKeys aKeyCode) const
{
	return !myCurrentState[(int)aKeyCode] && myPreviousState[(int)aKeyCode];
}

bool Input::IsMouseButtonDown(eKeys aMouseButtonCode) const
{
	return myCurrentState[(int)aMouseButtonCode] && myPreviousState[(int)aMouseButtonCode];
}

bool Input::IsMouseButtonPressed(eKeys aMouseButtonCode) const
{
	return IsKeyPressed(aMouseButtonCode);
}

bool Input::IsMouseButtonUp(eKeys aMouseButtonCode) const
{
	return IsKeyUp(aMouseButtonCode);
}

POINT Input::GetMousePosition() const
{
	return myCurrentMousePos;
}

POINT Input::GetMouseDelta() const
{
	POINT myDelta;
	myDelta.x = myCurrentMousePos.x - myPreviousMousePos.x;
	myDelta.y = myCurrentMousePos.y - myPreviousMousePos.y;
	return myDelta;
}

void Input::ResetMouseDelta()
{
	myCurrentMousePos = myTentativeMousePos;
}

bool Input::GetMouseInsideWindow() const
{
	return myMouseInsideWindow;
}

void Input::MouseInsideWindowUpdate(bool aState)
{
	myMouseInsideWindow = aState;
}

bool Input::UpdateEvents(const UINT message, const WPARAM wParam, const LPARAM lParam)
{
	switch (message)
	{
		// Keyboard
		case WM_KEYDOWN:
			myTentativeState[wParam] = true;
			return true;
		case WM_KEYUP:
			myTentativeState[wParam] = false;
			return true;

			// Mouse
		case WM_LBUTTONDOWN:
			myTentativeState[0x01] = true;
			return true;
		case WM_LBUTTONUP:
			myTentativeState[0x01] = false;
			return true;
		case WM_RBUTTONDOWN:
			myTentativeState[0x02] = true;
			return true;
		case WM_RBUTTONUP:
			myTentativeState[0x02] = false;
			return true;
		case WM_MBUTTONDOWN:
			myTentativeState[0x04] = true;
			return true;
		case WM_MBUTTONUP:
			myTentativeState[0x04] = false;
			return true;
		case WM_MOUSEMOVE:
			myTentativeMousePos.x = LOWORD(lParam);
			myTentativeMousePos.y = HIWORD(lParam);
			return true;
	}
}

void Input::UpdateStates()
{
	myPreviousState = myCurrentState;
	myCurrentState = myTentativeState;
	myPreviousMousePos = myCurrentMousePos;
	myCurrentMousePos = myTentativeMousePos;
}