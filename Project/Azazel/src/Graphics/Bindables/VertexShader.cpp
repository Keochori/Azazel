#include "pch.h"
#include "VertexShader.h"
#include <d3dcompiler.h>
#include "Graphics/Diagnostics/DXASSERT.h"

VertexShader::VertexShader()
{
}

void VertexShader::Create(DX11& aDX11)
{
	ComPtr<ID3DBlob> blob;
	HRASSERT(D3DReadFileToBlob(L"resources/shaders/Default_vs.cso", &blob));
	HRASSERT(aDX11.GetDevice()->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &myShader));
}

void VertexShader::Bind(DX11& aDX11)
{
    DXASSERT(aDX11.GetContext()->VSSetShader(myShader.Get(), nullptr, 0u));
}

ID3D11VertexShader* VertexShader::Get() const
{
    return myShader.Get();
}

ID3D11VertexShader* const* VertexShader::GetAddressOf() const
{
    return myShader.GetAddressOf();
}
