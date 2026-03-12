#include "pch.h"
#include "DX11.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include "DXGIInfoManager.h"
#include "DirectXMath.h"

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

	// Create depth stencil state
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	ComPtr<ID3D11DepthStencilState> depthStencilState;
	HRASSERT(myDevice->CreateDepthStencilState(&depthStencilDesc, &depthStencilState));

	// Bind depth stencil state
	DXASSERT(myContext->OMSetDepthStencilState(depthStencilState.Get(), 1u));

	// Create depth stencil texture
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

	// Create view of depth stencil texture
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0u;
	HRASSERT(myDevice->CreateDepthStencilView(depthStencilTexture.Get(), &dsvDesc, &myDSV));

	DXASSERT(myContext->OMSetRenderTargets(1u, myRTV.GetAddressOf(), myDSV.Get()));
}

DX11::~DX11()
{
}

void DX11::DrawCube(float angle, float x, float y, float z)
{
	// Create vertex buffer content
	struct Vertex
	{
		struct
		{
			float x;
			float y;
			float z;
		} pos;
	};

	const Vertex vertices[] = 
	{
		{-1.0f, -1.0f, -1.0f },
		{1.0f, -1.0f, -1.0f  },
		{-1.0f, 1.0f, -1.0f  },
		{1.0f, 1.0f, -1.0f   },
		{-1.0f, -1.0f, 1.0f  },
		{1.0f, -1.0f, 1.0f   },
		{-1.0f, 1.0f, 1.0f   },
		{1.0f, 1.0f, 1.0f	 },
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

	// Create Index Buffer
	const unsigned short indices[] = {
		0,3,1, 0,2,3, //back
		4,2,0, 4,6,2, //l-side
		5,1,3, 5,3,7, //r-side
		6,3,2, 6,7,3, //up
		4,1,5, 4,0,1, //down
		4,7,6, 4,5,7, //front
	};
	ComPtr<ID3D11Buffer> indexBuffer;
	bufferDesc = {};
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.CPUAccessFlags = 0u;
	bufferDesc.MiscFlags = 0u;
	bufferDesc.ByteWidth = sizeof(indices);
	bufferDesc.StructureByteStride = sizeof(unsigned short);

	subResData = {};
	subResData.pSysMem = indices;
	DXASSERT(myDevice->CreateBuffer(&bufferDesc, &subResData, &indexBuffer));

	// Bind index Buffer
	myContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

	// Create Constant Buffer
	struct ConstantBuffer
	{
		DirectX::XMMATRIX transform;
	};
	const ConstantBuffer cb =
	{
		{
			DirectX::XMMatrixTranspose(
				DirectX::XMMatrixRotationX(angle)*
				DirectX::XMMatrixRotationY(angle*0.1f)*
				DirectX::XMMatrixTranslation(x,y,z)*
				DirectX::XMMatrixPerspectiveLH(1.0f, GetScreenHeight() / GetScreenWidth(),0.5f,10.f)
			)
		}
	};
	ComPtr<ID3D11Buffer> constantBuffer;
	bufferDesc = {};
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0u;
	bufferDesc.ByteWidth = sizeof(cb);
	bufferDesc.StructureByteStride = sizeof(ConstantBuffer);

	subResData = {};
	subResData.pSysMem = &cb;
	DXASSERT(myDevice->CreateBuffer(&bufferDesc, &subResData, &constantBuffer));

	// Bind Constant Buffer
	myContext->VSSetConstantBuffers(0u, 1u, constantBuffer.GetAddressOf());

	// Cb2
	struct ConstantBuffer2
	{
		struct
		{
			float r;
			float g;
			float b;
			float a;
		} face_colors[6];
	};
	const ConstantBuffer2 cb2 =
	{
		{
			{1.0f,0.0f,1.0f},
			{1.0f,0.0f,0.0f},
			{0.0f,1.0f,0.0f},
			{0.0f,0.0f,1.0f},
			{1.0f,1.0f,0.0f},
			{0.0f,1.0f,1.0f},
		}
	};

	ComPtr<ID3D11Buffer> constantBuffer2;
	bufferDesc = {};
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0u;
	bufferDesc.ByteWidth = sizeof(cb2);
	bufferDesc.StructureByteStride = sizeof(ConstantBuffer2);

	subResData = {};
	subResData.pSysMem = &cb2;
	DXASSERT(myDevice->CreateBuffer(&bufferDesc, &subResData, &constantBuffer2));

	// Bind Constant Buffer
	myContext->PSSetConstantBuffers(0u, 1u, constantBuffer2.GetAddressOf());

	// Blob for loading shader data
	ComPtr<ID3DBlob> blob;

	// Create Pixel Shader
	ComPtr<ID3D11PixelShader> pixelShader;
	HRASSERT(D3DReadFileToBlob(L"resources/shaders/Default_ps.cso", &blob));
	HRASSERT(myDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixelShader));

	// Bind Pixel Shader
	DXASSERT(myContext->PSSetShader(pixelShader.Get(), nullptr, 0u));

	// Create Vertex Shader
	ComPtr<ID3D11VertexShader> vertexShader;
	HRASSERT(D3DReadFileToBlob(L"resources/shaders/Default_vs.cso", &blob));
	HRASSERT(myDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertexShader));

	// Bind Vertex Shader
	DXASSERT(myContext->VSSetShader(vertexShader.Get(), nullptr, 0u));

	// Create Input Layout
	ComPtr<ID3D11InputLayout> inputLayout;
	const D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
	};

	HRASSERT(myDevice->CreateInputLayout(inputElementDesc, (UINT)std::size(inputElementDesc), blob->GetBufferPointer(), blob->GetBufferSize(), &inputLayout));

	// Bind Input Layout
	DXASSERT(myContext->IASetInputLayout(inputLayout.Get()));

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
	DXASSERT(myContext->DrawIndexed((UINT)std::size(indices), 0u, 0u));
}

void DX11::BeginFrame()
{
	DXASSERT(myContext->OMSetRenderTargets(1u, myRTV.GetAddressOf(), myDSV.Get()));
}

void DX11::ClearBuffer(float r, float g, float b)
{
	const float color[] = { r,g,b,1.0f };
	DXASSERT(myContext->ClearRenderTargetView(myRTV.Get(), color));
	DXASSERT(myContext->ClearDepthStencilView(myDSV.Get(), D3D11_CLEAR_DEPTH,1.0f,0u));
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


