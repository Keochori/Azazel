#pragma once
#include "Graphics/Bindables/IBindable.h"

class Sampler : public IBindable<ID3D11SamplerState>
{
public:
	Sampler() = default;
	void Create(ComPtr<ID3D11Device>& aDevice) override;
	void Bind(ComPtr<ID3D11DeviceContext>& aContext) override;
	ID3D11SamplerState* Get() const override;
	ID3D11SamplerState* const* GetAddressOf() const override;

private:
	ComPtr<ID3D11SamplerState> mySamplerState;
};

