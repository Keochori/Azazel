#pragma once
#include <windows.h>
#include <d3d11.h>
#include <wrl.h>

using namespace Microsoft::WRL;

class DX11
{
public:
	DX11(HWND& aHWND);
	DX11(const DX11&) = delete;
	DX11& operator=(const DX11&) = delete;
	~DX11();

	void BeginFrame();
	void ClearBuffer(const float color[]);
	void EndFrame();

	void OnResize(UINT aWidth, UINT aHeight);

	ComPtr<ID3D11Device>& GetDevice();
	ComPtr<ID3D11DeviceContext>& GetContext();

	float GetScreenWidth() const;
	float GetScreenHeight() const;

private:
	void CreateDeviceAndSwapChainAndContext();
	void CreateRTV();
	void CreateDSV(UINT aWidth, UINT aHeight);
	void SetPrimitiveTopology();
	void SetViewPort(UINT aWidth, UINT aHeight);

	void SetDefaultVS();
	void SetDefaultPS();

	ComPtr<ID3D11Device> myDevice = nullptr;
	ComPtr<ID3D11DeviceContext> myContext = nullptr;
	ComPtr<IDXGISwapChain> mySwapChain = nullptr;
	ComPtr<ID3D11RenderTargetView> myRTV = nullptr;
	ComPtr<ID3D11DepthStencilView> myDSV = nullptr;

	HWND myHWND;
};

