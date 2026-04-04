#include "pch.h"
#include "DX11.h"
#include "Diagnostics/DXASSERT.h"
#include "Bindables/VertexShader.h"
#include "Bindables/PixelShader.h"

DX11::DX11(HWND& aHWND) : myHWND(aHWND)
{
	// Initialize DXGIInfoManager
#ifdef _DEBUG
	DXGIInfoManager::GetInstance().Init();
#endif

	CreateDeviceAndSwapChainAndContext();
	CreateRTV();
	CreateTextureResources(GetScreenWidth(), GetScreenHeight());
	CreateDSV(GetScreenWidth(), GetScreenHeight());

	SetPrimitiveTopology();
	SetViewPort(GetScreenWidth(), GetScreenHeight());
	SetDefaultVS();
	SetDefaultPS();
}

DX11::~DX11()
{
}

void DX11::PresentFrame()
{
	// Present frame
	DXASSERT(mySwapChain->Present(1, 0));
}

void DX11::BindRTV(bool aFullscreenMode)
{
	if (aFullscreenMode)
	{
		DXASSERT(myContext->OMSetRenderTargets(1u, myRTV.GetAddressOf(), myDSV.Get()));
	}
	else
	{
		DXASSERT(myContext->OMSetRenderTargets(1u, myRTV.GetAddressOf(), nullptr));
	}
}

void DX11::ClearRTV(bool aFullscreenMode)
{
	const float black[] = { 0,0,0,1 };
	const float gray[] = { 0.2f,0.2f,0.2f,1.0f };

	if (aFullscreenMode)
	{
		DXASSERT(myContext->ClearRenderTargetView(myRTV.Get(), gray));
		DXASSERT(myContext->ClearDepthStencilView(myDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u));
	}
	else
	{
		DXASSERT(myContext->ClearRenderTargetView(myRTV.Get(), black));
	}
}

void DX11::BindTextureRTV()
{
	DXASSERT(myContext->OMSetRenderTargets(1u, myTextureRTV.GetAddressOf(), myDSV.Get()));
}

void DX11::ClearTextureRTV()
{
	const float gray[] = { 0.2f,0.2f,0.2f,1.0f };

	DXASSERT(myContext->ClearRenderTargetView(myTextureRTV.Get(), gray));
	DXASSERT(myContext->ClearDepthStencilView(myDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u));
}

void DX11::CreateDeviceAndSwapChainAndContext()
{
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
	swapChainDesc.OutputWindow = myHWND;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.Flags = 0;

	// Create Device, Swapchain and Context
	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	HRASSERT(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		createDeviceFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&mySwapChain,
		&myDevice,
		nullptr,
		&myContext
	));
}

void DX11::CreateTextureResources(UINT aWidth, UINT aHeight)
{
	// Create texture
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = aWidth;
	textureDesc.Height = aHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	ComPtr<ID3D11Texture2D> texture2D;
	HRASSERT(myDevice->CreateTexture2D(&textureDesc, nullptr, &texture2D));

	// Create and bind RTV to texture
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = textureDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;

	HRASSERT(myDevice->CreateRenderTargetView(texture2D.Get(), &rtvDesc, &myTextureRTV));

	// Create and bind SRV to texture
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	HRASSERT(myDevice->CreateShaderResourceView(texture2D.Get(), &srvDesc, &myTextureSRV));
}

void DX11::CreateRTV()
{
	// Get Backbuffer
	ComPtr<ID3D11Resource> backBuffer = nullptr;
	HRASSERT(mySwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer));

	// Create RTV
	HRASSERT(myDevice->CreateRenderTargetView(
		backBuffer.Get(),
		nullptr,
		&myRTV
	));
}

void DX11::CreateDSV(UINT aWidth, UINT aHeight)
{
	// Create Depth Stencil State
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	ComPtr<ID3D11DepthStencilState> depthStencilState;
	HRASSERT(myDevice->CreateDepthStencilState(&depthStencilDesc, &depthStencilState));

	// Bind Depth Stencil State
	DXASSERT(myContext->OMSetDepthStencilState(depthStencilState.Get(), 1u));

	// Create Depth Stencil Texture
	ComPtr<ID3D11Texture2D> depthStencilTexture;
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = aWidth;
	textureDesc.Height = aHeight;
	textureDesc.MipLevels = 1u;
	textureDesc.ArraySize = 1u;
	textureDesc.Format = DXGI_FORMAT_D32_FLOAT;
	textureDesc.SampleDesc.Count = 1u;
	textureDesc.SampleDesc.Quality = 0u;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	HRASSERT(myDevice->CreateTexture2D(&textureDesc, nullptr, &depthStencilTexture));

	// Create DSV
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0u;
	HRASSERT(myDevice->CreateDepthStencilView(depthStencilTexture.Get(), &dsvDesc, &myDSV));
}

void DX11::SetDefaultVS()
{
	VertexShader vertexShader;
	vertexShader.Create(myDevice);
	vertexShader.Bind(myContext);
}

void DX11::SetDefaultPS()
{
	PixelShader pixelShader;
	pixelShader.Create(myDevice);
	pixelShader.Bind(myContext);
}

void DX11::SetPrimitiveTopology()
{
	DXASSERT(myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
}

void DX11::SetViewPort(UINT aWidth, UINT aHeight)
{
	D3D11_VIEWPORT viewport;
	viewport.Width = aWidth;
	viewport.Height = aHeight;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	DXASSERT(myContext->RSSetViewports(1u, &viewport));
}

void DX11::OnWindowResize(UINT aWidth, UINT aHeight, bool aFullScreenMode)
{
	if (aWidth <= 0 || aHeight <= 0)
		return;

	myContext->OMSetRenderTargets(0u, nullptr, nullptr);
	myRTV.Reset();
	if (aFullScreenMode)
		myDSV.Reset();

	mySwapChain->ResizeBuffers(0u, aWidth, aHeight, DXGI_FORMAT_UNKNOWN, 0u);
	CreateRTV();
	if (aFullScreenMode)
	{
		CreateDSV(aWidth, aHeight);
		SetViewPort(aWidth, aHeight);
	}
}

void DX11::OnTextureResize(UINT aWidth, UINT aHeight)
{
	if (aWidth <= 0 || aHeight <= 0)
		return;

	myContext->OMSetRenderTargets(0u, nullptr, nullptr);
	myTextureRTV.Reset();
	myTextureSRV.Reset();
	myDSV.Reset();

	CreateTextureResources(aWidth, aHeight);
	CreateDSV(aWidth, aHeight);
	SetViewPort(aWidth, aHeight);
}

ComPtr<ID3D11Device>& DX11::GetDevice()
{
	return myDevice;
}

ComPtr<ID3D11DeviceContext>& DX11::GetContext()
{
	return myContext;
}

ComPtr<ID3D11ShaderResourceView>& DX11::GetTextureSRV()
{
	return myTextureSRV;
}

float DX11::GetScreenWidth() const
{
	RECT rect;
	GetClientRect(myHWND, &rect);

	return (float)(rect.right - rect.left);
}

float DX11::GetScreenHeight() const
{
	RECT rect;
	GetClientRect(myHWND, &rect);

	return (float)(rect.bottom - rect.top);
}


