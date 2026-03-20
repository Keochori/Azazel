#pragma once
#include "Graphics/Bindables/IBindable.h"
#include <vector>

class IndexBuffer : public IBindable<ID3D11Buffer>
{
public:
	IndexBuffer(const std::vector<unsigned short>& aIndices);
	void Create(ComPtr<ID3D11Device>& aDevice) override;
	void Bind(ComPtr<ID3D11DeviceContext>& aContext) override;
	ID3D11Buffer* Get() const override;
	ID3D11Buffer* const* GetAddressOf() const override;
	const std::vector<unsigned short> GetIndices() const;

private:
	const std::vector<unsigned short> myIndices;
	ComPtr<ID3D11Buffer> myBuffer;
};

