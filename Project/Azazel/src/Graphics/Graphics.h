#pragma once
#include <windows.h>
#include <d3d11.h>

class Graphics
{
public:
	Graphics(HWND aHWND);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();

	void EndFrame();
	void ClearBuffer(float r, float g, float b)
	{
		const float color[] = { r,g,b,1.0f };
		myContext->ClearRenderTargetView(myRTV, color);
	}

private:
	ID3D11Device* myDevice = nullptr;
	ID3D11DeviceContext* myContext = nullptr;
	IDXGISwapChain* mySwapChain = nullptr;
	ID3D11RenderTargetView* myRTV = nullptr;
};

