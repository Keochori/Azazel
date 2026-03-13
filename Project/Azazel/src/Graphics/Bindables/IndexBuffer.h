#pragma once
#include "Graphics/Bindables/IBindable.h"
#include <wrl.h>
#include <vector>

using namespace Microsoft::WRL;

class IndexBuffer : public IBindable<ID3D11Buffer>
{
public:
	IndexBuffer(const std::vector<unsigned short>& aIndices);
	void Create(DX11& aDX11) override;
	void Bind(DX11& aDX11) override;
	ID3D11Buffer* Get() const override;
	ID3D11Buffer* const* GetAddressOf() const override;

private:
	const std::vector<unsigned short> myIndices;
	ComPtr<ID3D11Buffer> myBuffer;
};

