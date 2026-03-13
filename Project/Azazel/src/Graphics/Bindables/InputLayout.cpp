#include "pch.h"
#include "InputLayout.h"
#include <d3dcompiler.h>
#include "Graphics/Diagnostics/DXASSERT.h"

InputLayout::InputLayout(const std::vector<D3D11_INPUT_ELEMENT_DESC>& aInputElementDesc) : myInputElementDesc(aInputElementDesc)
{
}

void InputLayout::Create(DX11& aDX11)
{
	ComPtr<ID3DBlob> blob;
	HRASSERT(D3DReadFileToBlob(L"resources/shaders/Default_vs.cso", &blob));
	HRASSERT(aDX11.GetDevice()->CreateInputLayout(myInputElementDesc.data(), (UINT)myInputElementDesc.size(), blob->GetBufferPointer(), blob->GetBufferSize(), &myInputLayout));
}

void InputLayout::Bind(DX11& aDX11)
{
	DXASSERT(aDX11.GetContext()->IASetInputLayout(myInputLayout.Get()));
}

ID3D11InputLayout* InputLayout::Get() const
{
	return myInputLayout.Get();
}

ID3D11InputLayout* const* InputLayout::GetAddressOf() const
{
	return myInputLayout.GetAddressOf();
}
