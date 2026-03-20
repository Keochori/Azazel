#include "pch.h"
#include "PixelShader.h"
#include <d3dcompiler.h>
#include "Graphics/Diagnostics/DXASSERT.h"

PixelShader::PixelShader()
{
}

void PixelShader::Create(ComPtr<ID3D11Device>& aDevice)
{
	ComPtr<ID3DBlob> blob;
	HRASSERT(D3DReadFileToBlob(L"resources/shaders/Default_ps.cso", &blob));
	HRASSERT(aDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &myShader));
}

void PixelShader::Bind(ComPtr<ID3D11DeviceContext>& aContext)
{
	DXASSERT(aContext->PSSetShader(myShader.Get(), nullptr, 0u));
}

ID3D11PixelShader* PixelShader::Get() const
{
	return myShader.Get();
}

ID3D11PixelShader* const* PixelShader::GetAddressOf() const
{
	return myShader.GetAddressOf();
}
