#pragma once
#include "Graphics/Bindables/IBindable.h"
#include "Vertex.h"
#include <wrl.h>
#include <vector>

using namespace Microsoft::WRL;

class VertexBuffer : public IBindable<ID3D11Buffer>
{
public:
	VertexBuffer(const std::vector<Vertex>& aVertices);
	void Create(DX11& aDX11) override;
	void Bind(DX11& aDX11) override;
	ID3D11Buffer* Get() const override;
	ID3D11Buffer* const* GetAddressOf() const override;

private:
	const std::vector<Vertex> myVertices;
	ComPtr<ID3D11Buffer> myBuffer;
};

