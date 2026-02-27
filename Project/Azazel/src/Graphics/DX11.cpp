#include "pch.h"
#include "DX11.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include "DXGIInfoManager.h"

DX11::DX11(HWND& aHWND) : myHWND(aHWND)
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

void DX11::DrawTestTriangle()
{
	// Create vertex buffer content
	struct Vertex
	{
		float x;
		float y;
	};

	const Vertex vertices[] = {
		{0.0f, 0.5f},
		{0.5f, -0.5f},
		{-0.5f, -0.5f}
	};

	// Create Vertex Buffer
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.CPUAccessFlags = 0u;
	bufferDesc.MiscFlags = 0u;
	bufferDesc.ByteWidth = sizeof(vertices);
	bufferDesc.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA subResData = {};
	subResData.pSysMem = vertices;

	ComPtr<ID3D11Buffer> vertexBuffer;
	HRASSERT(myDevice->CreateBuffer(&bufferDesc, &subResData, &vertexBuffer));

	// Bind Vertex Buffer to pipeline
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	DXASSERT(myContext->IASetVertexBuffers(0u, 1u, vertexBuffer.GetAddressOf(), &stride, &offset));

	// Blob for loading shader data
	ComPtr<ID3DBlob> blob;

	// Create Pixel Shader
	ComPtr<ID3D11PixelShader> pixelShader;
	HRASSERT(D3DReadFileToBlob(L"Resources/Shaders/Default_ps.cso", &blob));
	HRASSERT(myDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixelShader));

	// Bind Pixel Shader
	DXASSERT(myContext->PSSetShader(pixelShader.Get(), nullptr, 0u));

	// Create Vertex Shader
	ComPtr<ID3D11VertexShader> vertexShader;
	HRASSERT(D3DReadFileToBlob(L"Resources/Shaders/Default_vs.cso", &blob));
	HRASSERT(myDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertexShader));

	// Bind Vertex Shader
	DXASSERT(myContext->VSSetShader(vertexShader.Get(), nullptr, 0u));

	// Create Input Layout
	ComPtr<ID3D11InputLayout> inputLayout;
	const D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
		{"POSITION",0,DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0}
	};

	HRASSERT(myDevice->CreateInputLayout(inputElementDesc, (UINT)std::size(inputElementDesc), blob->GetBufferPointer(), blob->GetBufferSize(), &inputLayout));

	// Bind Input Layout
	DXASSERT(myContext->IASetInputLayout(inputLayout.Get()));

	// Bind Render Target
	DXASSERT(myContext->OMSetRenderTargets(1u, myRTV.GetAddressOf(), nullptr));

	// Set Primitive Topology
	DXASSERT(myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	// Configure viewport
	D3D11_VIEWPORT viewport;
	viewport.Width = GetScreenWidth();
	viewport.Height = GetScreenHeight();
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	DXASSERT(myContext->RSSetViewports(1u, &viewport));

	// Draw
	DXASSERT(myContext->Draw((UINT)std::size(vertices), 0u));
}

void DX11::ClearBuffer(float r, float g, float b)
{
	const float color[] = { r,g,b,1.0f };
	DXASSERT(myContext->ClearRenderTargetView(myRTV.Get(), color));
}

void DX11::EndFrame()
{
	DXASSERT(mySwapChain->Present(1, 0));
}

void DX11::HRESULTCheck(HRESULT aHr, const char* aFile, int aLine)
{
	bool HRFailed = FAILED(aHr);

	DXAssertMessages(aFile, aLine);

	Assert(!HRFailed);
}

void DX11::DXAssertMessages(const char* aFile, int aLine)
{
	bool error = false;
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
				error = true;
				break;
		}
	}

	myDXGIInfoManager->UpdateInfoQueuePosition();
	Assert(!error);
}

int DX11::GetScreenWidth() const
{
	RECT rect;
	GetClientRect(myHWND, &rect);

	return rect.right - rect.left;
}

int DX11::GetScreenHeight() const
{
	RECT rect;
	GetClientRect(myHWND, &rect);

	return rect.bottom - rect.top;
}


