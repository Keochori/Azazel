#pragma once
#include "Graphics/Bindables/IBindable.h"
#include <wrl.h>

using namespace Microsoft::WRL;

class PixelShader : public IBindable<ID3D11PixelShader>
{
public:
	PixelShader();
	void Create(DX11& aDX11) override;
	void Bind(DX11& aDX11) override;
	ID3D11PixelShader* Get() const override;
	ID3D11PixelShader* const* GetAddressOf() const override;

private:
	ComPtr<ID3D11PixelShader> myShader;
};

