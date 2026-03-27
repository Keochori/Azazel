#pragma once
#include "Graphics/Bindables/IBindable.h"
#include "directxtex/DirectXTex.h"

class SRV : public IBindable<ID3D11ShaderResourceView>
{
public:
	SRV(DirectX::ScratchImage aImage);
	void Create(ComPtr<ID3D11Device>& aDevice) override;
	void Bind(ComPtr<ID3D11DeviceContext>& aContext) override;
	ID3D11ShaderResourceView* Get() const override;
	ID3D11ShaderResourceView* const* GetAddressOf() const override;

private:
	DirectX::ScratchImage myImage;
	ComPtr<ID3D11ShaderResourceView> mySRV;
};

