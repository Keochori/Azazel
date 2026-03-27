#include "pch.h"
#include "SRV.h"
#include "Graphics/Diagnostics/DXASSERT.h"

SRV::SRV(DirectX::ScratchImage aImage) : myImage(std::move(aImage))
{
}

void SRV::Create(ComPtr<ID3D11Device>& aDevice)
{
	ComPtr<ID3D11Resource> texture;

	HRASSERT(DirectX::CreateTexture(aDevice.Get(), myImage.GetImages(), myImage.GetImageCount(), myImage.GetMetadata(), &texture));

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = myImage.GetMetadata().format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = myImage.GetMetadata().mipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0u;
	srvDesc.Texture2D.MipLevels = 1u;

	HRASSERT(aDevice->CreateShaderResourceView(texture.Get(), &srvDesc, &mySRV));
}

void SRV::Bind(ComPtr<ID3D11DeviceContext>& aContext)
{
	DXASSERT(aContext->PSSetShaderResources(0u, 1u, mySRV.GetAddressOf()));
}

ID3D11ShaderResourceView* SRV::Get() const
{
	return mySRV.Get();
}

ID3D11ShaderResourceView* const* SRV::GetAddressOf() const
{
	return mySRV.GetAddressOf();
}
