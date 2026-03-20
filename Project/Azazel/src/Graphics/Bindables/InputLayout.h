#pragma once
#include "Graphics/Bindables/IBindable.h"
#include <vector>

class InputLayout : public IBindable<ID3D11InputLayout>
{
public:
	InputLayout(const std::vector<D3D11_INPUT_ELEMENT_DESC>& aInputElementDesc);
	void Create(ComPtr<ID3D11Device>& aDevice) override;
	void Bind(ComPtr<ID3D11DeviceContext>& aContext) override;
	ID3D11InputLayout* Get() const override;
	ID3D11InputLayout* const* GetAddressOf() const override;

private:
	const std::vector<D3D11_INPUT_ELEMENT_DESC> myInputElementDesc;
	ComPtr<ID3D11InputLayout> myInputLayout;
};

