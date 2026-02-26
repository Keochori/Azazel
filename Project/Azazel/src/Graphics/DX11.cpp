#include "pch.h"
#include "DX11.h"
#include <d3d11.h>
#include "DXGIInfoManager.h"

DX11::DX11(HWND aHWND)
{
	// Initialize DXGIInfoManager
	myDXGIInfoManager = std::make_unique<DXGIInfoManager>();
	myDXGIInfoManager->UpdateInfoQueuePosition();

	// Swap Chain Desc
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferDesc.Width = 0;
	swapChainDesc.BufferDesc.Height = 0;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.OutputWindow = aHWND;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.Flags = 0;

	// Create Device, Swapchain and Context
	HRASSERT(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&mySwapChain,
		&myDevice,
		nullptr,
		&myContext
	));

	// Create RTV
	ComPtr<ID3D11Resource> backBuffer = nullptr;
	HRASSERT(mySwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer));
	if (!backBuffer)
		return;

	HRASSERT(myDevice->CreateRenderTargetView(
		backBuffer.Get(),
		nullptr,
		&myRTV
	));
}

DX11::~DX11()
{
}

void DX11::ClearBuffer(float r, float g, float b)
{
	const float color[] = { r,g,b,1.0f };
	myContext->ClearRenderTargetView(myRTV.Get(), color);
}

void DX11::EndFrame()
{
	HRASSERT(mySwapChain->Present(1, 0));
}

void DX11::HRESULTCheck(HRESULT aHr, const char* aFile, int aLine)
{
	bool HRSuccess = !FAILED(aHr);

	std::vector<DXGIInfoMessage> messages = myDXGIInfoManager->GetMessages();
	for (DXGIInfoMessage& message : messages)
	{
		switch (message.mySeverity)
		{
			case Severity::eWarning:
				LOG::Print(message.myDescription, aFile, aLine, LogType::eWarning);
				break;

			case Severity::eError:
				LOG::Print(message.myDescription, aFile, aLine, LogType::eError);
		}
	}

	myDXGIInfoManager->UpdateInfoQueuePosition();

	Assert(HRSuccess);
}
