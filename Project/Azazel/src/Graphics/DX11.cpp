#include "pch.h"
#include "DX11.h"
#include "DirectXMath.h"
#include "Diagnostics/DXASSERT.h"
#include "Bindables/VertexShader.h"
#include "Bindables/PixelShader.h"

// Remove later
#include "Bindables/InputLayout.h"
#include "Bindables/VertexBuffer.h"
#include "Bindables/IndexBuffer.h"
#include "Bindables/ConstantBuffer.hpp"

DX11::DX11(HWND& aHWND) : myHWND(aHWND)
{
	// Initialize DXGIInfoManager
	DXGIInfoManager::GetInstance().Init();

	CreateDeviceAndSwapChainAndContext();
	CreateRTV();
	CreateDSV();

	SetPrimitiveTopology();
	SetViewPort();
	SetDefaultVS();
	SetDefaultPS();
}

DX11::~DX11()
{
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

void DX11::CreateDSV()
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
	textureDesc.Width = GetScreenWidth();
	textureDesc.Height = GetScreenHeight();
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
	vertexShader.Create(*this);
	vertexShader.Bind(*this);
}

void DX11::SetDefaultPS()
{
	PixelShader pixelShader;
	pixelShader.Create(*this);
	pixelShader.Bind(*this);
}

void DX11::SetPrimitiveTopology()
{
	DXASSERT(myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
}

void DX11::SetViewPort()
{
	D3D11_VIEWPORT viewport;
	viewport.Width = GetScreenWidth();
	viewport.Height = GetScreenHeight();
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	DXASSERT(myContext->RSSetViewports(1u, &viewport));
}

void DX11::BeginFrame()
{
	// Bind RTV and DSV
	DXASSERT(myContext->OMSetRenderTargets(1u, myRTV.GetAddressOf(), myDSV.Get()));
}

void DX11::ClearBuffer(const float color[])
{
	// Clear RTV and DSV
	DXASSERT(myContext->ClearRenderTargetView(myRTV.Get(), color));
	DXASSERT(myContext->ClearDepthStencilView(myDSV.Get(), D3D11_CLEAR_DEPTH,1.0f,0u));
}

void DX11::EndFrame()
{
	// Present frame
	DXASSERT(mySwapChain->Present(1, 0));
}

void DX11::DrawCube(float angle, float x, float y, float z)
{
	// Create and Bind Vertex Buffer
	VertexBuffer vertexBuffer({
		{-1.0f, -1.0f, -1.0f },
		{1.0f, -1.0f, -1.0f  },
		{-1.0f, 1.0f, -1.0f  },
		{1.0f, 1.0f, -1.0f   },
		{-1.0f, -1.0f, 1.0f  },
		{1.0f, -1.0f, 1.0f   },
		{-1.0f, 1.0f, 1.0f   },
		{1.0f, 1.0f, 1.0f	 },
		});
	vertexBuffer.Create(*this);
	vertexBuffer.Bind(*this);

	// Create and Bind Index Buffer
	std::vector<unsigned short> indices = {
		0,3,1, 0,2,3, //back
		4,2,0, 4,6,2, //l-side
		5,1,3, 5,3,7, //r-side
		6,3,2, 6,7,3, //up
		4,1,5, 4,0,1, //down
		4,7,6, 4,5,7, //front
	};
	IndexBuffer indexBuffer(indices);
	indexBuffer.Create(*this);
	indexBuffer.Bind(*this);

	// Create and Bind Transform Buffer
	ConstantBuffer<TransformBuffer> transformBuffer(
		eBindType::VS, 
		TransformBuffer({
			DirectX::XMMatrixTranspose(
			DirectX::XMMatrixRotationX(angle) *
			DirectX::XMMatrixRotationY(angle * 0.1f) *
			DirectX::XMMatrixTranslation(x, y, z) *
			DirectX::XMMatrixPerspectiveLH(1.0f, GetScreenHeight() / GetScreenWidth(), 0.5f, 10.f))
		})
	);

	transformBuffer.Create(*this);
	transformBuffer.Bind(*this);

	// Create and Bind Color Buffer
	ConstantBuffer<ColorBuffer> colorBuffer(
		eBindType::PS, 
		ColorBuffer({
			DirectX::XMVectorSet(1.0f,0.0f,1.0f,0.0f),
			DirectX::XMVectorSet(1.0f,0.0f,0.0f,0.0f),
			DirectX::XMVectorSet(0.0f,1.0f,0.0f,0.0f),
			DirectX::XMVectorSet(0.0f,0.0f,1.0f,0.0f),
			DirectX::XMVectorSet(1.0f,1.0f,0.0f,0.0f),
			DirectX::XMVectorSet(0.0f,1.0f,1.0f,0.0f)
		})
	);
	colorBuffer.Create(*this);
	colorBuffer.Bind(*this);

	// Create Input Layout
	InputLayout inputLayout(
		{
			{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		});

	inputLayout.Create(*this);
	inputLayout.Bind(*this);

	// Draw
	DXASSERT(myContext->DrawIndexed((UINT)indices.size(), 0u, 0u));
}

ComPtr<ID3D11Device> DX11::GetDevice() const
{
	return myDevice;
}

ComPtr<ID3D11DeviceContext> DX11::GetContext() const
{
	return myContext;
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


