#pragma once
#include "Graphics/Bindables/IBindable.h"
#include <wrl.h>

using namespace Microsoft::WRL;

class VertexShader : public IBindable<ID3D11VertexShader>
{
public:
	VertexShader();
	void Create(DX11& aDX11) override;
	void Bind(DX11& aDX11) override;
	ID3D11VertexShader* Get() const override;
	ID3D11VertexShader* const* GetAddressOf() const override;

private:
	ComPtr<ID3D11VertexShader> myShader;
};

