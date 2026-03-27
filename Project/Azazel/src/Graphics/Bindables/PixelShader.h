#pragma once
#include "Graphics/Bindables/IBindable.h"

class PixelShader : public IBindable<ID3D11PixelShader>
{
public:
	PixelShader() = default;
	void Create(ComPtr<ID3D11Device>& aDevice) override;
	void Bind(ComPtr<ID3D11DeviceContext>& aContext) override;
	ID3D11PixelShader* Get() const override;
	ID3D11PixelShader* const* GetAddressOf() const override;

private:
	ComPtr<ID3D11PixelShader> myShader;
};

