#pragma once
#include "Graphics/Bindables/IBindable.h"
#include "Vertex.h"
#include <vector>

class VertexBuffer : public IBindable<ID3D11Buffer>
{
public:
	VertexBuffer(const std::vector<Vertex>& aVertices);
	void Create(ComPtr<ID3D11Device>& aDevice) override;
	void Bind(ComPtr<ID3D11DeviceContext>& aContext) override;
	ID3D11Buffer* Get() const override;
	ID3D11Buffer* const* GetAddressOf() const override;

private:
	const std::vector<Vertex> myVertices;
	ComPtr<ID3D11Buffer> myBuffer;
};

