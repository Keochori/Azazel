#pragma once
#include <windows.h>
#include <d3d11.h>
#include <wrl.h>

// Remove later
#include "Assets/AssetHandler.h"

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

	void DrawCube(float angle, float x, float y, float z);
	void DrawGremlin(float angle, float x, float y, float z);

	ComPtr<ID3D11Device> GetDevice() const;
	ComPtr<ID3D11DeviceContext> GetContext() const;

private:
	void CreateDeviceAndSwapChainAndContext();
	void CreateRTV();
	void CreateDSV();
	void SetPrimitiveTopology();
	void SetViewPort();

	void SetDefaultVS();
	void SetDefaultPS();

	float GetScreenWidth() const;
	float GetScreenHeight() const;

	ComPtr<ID3D11Device> myDevice = nullptr;
	ComPtr<ID3D11DeviceContext> myContext = nullptr;
	ComPtr<IDXGISwapChain> mySwapChain = nullptr;
	ComPtr<ID3D11RenderTargetView> myRTV = nullptr;
	ComPtr<ID3D11DepthStencilView> myDSV = nullptr;

	// Remove later
	AssetHandler myAssetHandler;

	HWND myHWND;
};

