#pragma once
#include "Graphics/Bindables/IBindable.h"

class VertexShader : public IBindable<ID3D11VertexShader>
{
public:
	VertexShader() = default;
	void Create(ComPtr<ID3D11Device>& aDevice) override;
	void Bind(ComPtr<ID3D11DeviceContext>& aContext) override;
	ID3D11VertexShader* Get() const override;
	ID3D11VertexShader* const* GetAddressOf() const override;

private:
	ComPtr<ID3D11VertexShader> myShader;
};

