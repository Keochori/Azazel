#include "pch.h"
#include "PixelShader.h"
#include <d3dcompiler.h>
#include "Graphics/Diagnostics/DXASSERT.h"

PixelShader::PixelShader()
{
}

void PixelShader::Create(DX11& aDX11)
{
	ComPtr<ID3DBlob> blob;
	HRASSERT(D3DReadFileToBlob(L"resources/shaders/Default_ps.cso", &blob));
	HRASSERT(aDX11.GetDevice()->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &myShader));
}

void PixelShader::Bind(DX11& aDX11)
{
	DXASSERT(aDX11.GetContext()->PSSetShader(myShader.Get(), nullptr, 0u));
}

ID3D11PixelShader* PixelShader::Get() const
{
	return myShader.Get();
}

ID3D11PixelShader* const* PixelShader::GetAddressOf() const
{
	return myShader.GetAddressOf();
}
