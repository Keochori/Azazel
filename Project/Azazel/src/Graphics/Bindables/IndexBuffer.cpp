#include "pch.h"
#include "IndexBuffer.h"
#include "Graphics/Diagnostics/DXASSERT.h"

IndexBuffer::IndexBuffer(const std::vector<unsigned short>& aIndices) : myIndices(aIndices)
{
}

void IndexBuffer::Create(ComPtr<ID3D11Device>& aDevice)
{
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc = {};
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.CPUAccessFlags = 0u;
	bufferDesc.MiscFlags = 0u;
	bufferDesc.ByteWidth = UINT(sizeof(unsigned short) * myIndices.size());
	bufferDesc.StructureByteStride = sizeof(unsigned short);

	D3D11_SUBRESOURCE_DATA subResData = {};
	subResData = {};
	subResData.pSysMem = myIndices.data();
	HRASSERT(aDevice->CreateBuffer(&bufferDesc, &subResData, &myBuffer));
}

void IndexBuffer::Bind(ComPtr<ID3D11DeviceContext>& aContext)
{
	DXASSERT(aContext->IASetIndexBuffer(myBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u));
}

ID3D11Buffer* IndexBuffer::Get() const
{
	return myBuffer.Get();
}

ID3D11Buffer* const* IndexBuffer::GetAddressOf() const
{
	return myBuffer.GetAddressOf();
}

const std::vector<unsigned short> IndexBuffer::GetIndices() const
{
	return myIndices;
}
