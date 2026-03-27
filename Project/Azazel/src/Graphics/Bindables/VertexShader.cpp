#include "pch.h"
#include "VertexShader.h"
#include <d3dcompiler.h>
#include "Graphics/Diagnostics/DXASSERT.h"

void VertexShader::Create(ComPtr<ID3D11Device>& aDevice)
{
	ComPtr<ID3DBlob> blob;
	HRASSERT(D3DReadFileToBlob(L"resources/shaders/Default_vs.cso", &blob));
	HRASSERT(aDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &myShader));
}

void VertexShader::Bind(ComPtr<ID3D11DeviceContext>& aContext)
{
    DXASSERT(aContext->VSSetShader(myShader.Get(), nullptr, 0u));
}

ID3D11VertexShader* VertexShader::Get() const
{
    return myShader.Get();
}

ID3D11VertexShader* const* VertexShader::GetAddressOf() const
{
    return myShader.GetAddressOf();
}
