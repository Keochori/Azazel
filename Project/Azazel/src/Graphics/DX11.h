#pragma once
#include <windows.h>
#include <memory>
#include <wrl.h>

using namespace Microsoft::WRL;

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;
class DXGIInfoManager;

class DX11
{
public:
	DX11(HWND aHWND);
	DX11(const DX11&) = delete;
	DX11& operator=(const DX11&) = delete;
	~DX11();

	// Remove Later
	void ClearBuffer(float r, float g, float b);

	void EndFrame();


private:
#define HRASSERT(hr) HRESULTCheck(hr, __FILE__, __LINE__)
	void HRESULTCheck(HRESULT aHr, const char* aFile, int aLine);

	ComPtr<ID3D11Device> myDevice = nullptr;
	ComPtr<ID3D11DeviceContext> myContext = nullptr;
	ComPtr<IDXGISwapChain> mySwapChain = nullptr;
	ComPtr<ID3D11RenderTargetView> myRTV = nullptr;
	std::unique_ptr<DXGIInfoManager> myDXGIInfoManager;
};

