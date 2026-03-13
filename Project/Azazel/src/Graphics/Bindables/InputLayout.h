#pragma once
#include "Graphics/Bindables/IBindable.h"
#include <wrl.h>
#include <vector>

using namespace Microsoft::WRL;

class InputLayout : public IBindable<ID3D11InputLayout>
{
public:
	InputLayout(const std::vector<D3D11_INPUT_ELEMENT_DESC>& aInputElementDesc);
	void Create(DX11& aDX11) override;
	void Bind(DX11& aDX11) override;
	ID3D11InputLayout* Get() const override;
	ID3D11InputLayout* const* GetAddressOf() const override;

private:
	const std::vector<D3D11_INPUT_ELEMENT_DESC> myInputElementDesc;
	ComPtr<ID3D11InputLayout> myInputLayout;
};

