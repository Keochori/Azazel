#include "pch.h"
#include "VertexBuffer.h"
#include "Graphics/Diagnostics/DXASSERT.h"

VertexBuffer::VertexBuffer(const std::vector<Vertex>& aVertices) : myVertices(aVertices)
{
}

void VertexBuffer::Create(DX11& aDX11)
{
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.CPUAccessFlags = 0u;
	bufferDesc.MiscFlags = 0u;
	bufferDesc.ByteWidth = UINT(sizeof(Vertex) * myVertices.size());
	bufferDesc.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA subResData = {};
	subResData.pSysMem = myVertices.data();

	HRASSERT(aDX11.GetDevice()->CreateBuffer(&bufferDesc, &subResData, &myBuffer));
}

void VertexBuffer::Bind(DX11& aDX11)
{
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	DXASSERT(aDX11.GetContext()->IASetVertexBuffers(0u, 1u, myBuffer.GetAddressOf(), &stride, &offset));
}

ID3D11Buffer* VertexBuffer::Get() const
{
	return myBuffer.Get();
}

ID3D11Buffer* const* VertexBuffer::GetAddressOf() const
{
	return myBuffer.GetAddressOf();
}
