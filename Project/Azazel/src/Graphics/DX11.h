#pragma once
#include <windows.h>
#include <memory>
#include <wrl.h>

using namespace Microsoft::WRL;

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
class DXGIInfoManager;

class DX11
{
public:
	DX11(HWND& aHWND);
	DX11(const DX11&) = delete;
	DX11& operator=(const DX11&) = delete;
	~DX11();

	void DrawCube(float angle, float x, float y, float z);

	void BeginFrame();
	void ClearBuffer(float r, float g, float b);
	void EndFrame();

private:
#define HRASSERT(hr) HRESULTCheck(hr, __FILE__, __LINE__)
#define DXASSERT(func) (func); DXAssertMessages(__FILE__, __LINE__)
	void HRESULTCheck(HRESULT aHr, const char* aFile, int aLine);
	void DXAssertMessages(const char* aFile, int aLine);
	float GetScreenWidth() const;
	float GetScreenHeight() const;

	ComPtr<ID3D11Device> myDevice = nullptr;
	ComPtr<ID3D11DeviceContext> myContext = nullptr;
	ComPtr<IDXGISwapChain> mySwapChain = nullptr;
	ComPtr<ID3D11RenderTargetView> myRTV = nullptr;
	ComPtr<ID3D11DepthStencilView> myDSV = nullptr;
	std::unique_ptr<DXGIInfoManager> myDXGIInfoManager;

	HWND myHWND;
};

