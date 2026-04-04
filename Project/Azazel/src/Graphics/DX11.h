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

	void PresentFrame();
	void BindRTV(bool aFullscreenMode = false);
	void ClearRTV(bool aFullscreenMode = false);
	void BindTextureRTV();
	void ClearTextureRTV();
	void SetViewPort(UINT aWidth, UINT aHeight);

	void OnWindowResize(UINT aWidth, UINT aHeight, bool aFullScreenMode = false);
	void OnTextureResize(UINT aWidth, UINT aHeight);

	ComPtr<ID3D11Device>& GetDevice();
	ComPtr<ID3D11DeviceContext>& GetContext();
	ComPtr<ID3D11ShaderResourceView>& GetTextureSRV();

	float GetScreenWidth() const;
	float GetScreenHeight() const;

private:
	void CreateDeviceAndSwapChainAndContext();
	void CreateRTV();
	void CreateDSV(UINT aWidth, UINT aHeight);
	void CreateTextureResources(UINT aWidth, UINT aHeight);

	void SetPrimitiveTopology();
	void SetDefaultVS();
	void SetDefaultPS();

	ComPtr<ID3D11Device> myDevice;
	ComPtr<ID3D11DeviceContext> myContext;
	ComPtr<IDXGISwapChain> mySwapChain;
	ComPtr<ID3D11RenderTargetView> myRTV;
	ComPtr<ID3D11DepthStencilView> myDSV;

	// Texture that we can render to and display in a imgui tab
	ComPtr<ID3D11RenderTargetView> myTextureRTV;
	ComPtr<ID3D11ShaderResourceView> myTextureSRV;

	HWND myHWND;
};

