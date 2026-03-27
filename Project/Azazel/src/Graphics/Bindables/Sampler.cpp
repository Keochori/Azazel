#include "pch.h"
#include "Sampler.h"
#include "Graphics/Diagnostics/DXASSERT.h"

void Sampler::Create(ComPtr<ID3D11Device>& aDevice)
{
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	HRASSERT(aDevice->CreateSamplerState(&samplerDesc, &mySamplerState));
}

void Sampler::Bind(ComPtr<ID3D11DeviceContext>& aContext)
{
	DXASSERT(aContext->PSSetSamplers(0u, 1u, mySamplerState.GetAddressOf()));
}

ID3D11SamplerState* Sampler::Get() const
{
	return mySamplerState.Get();
}

ID3D11SamplerState* const* Sampler::GetAddressOf() const
{
	return mySamplerState.GetAddressOf();
}
