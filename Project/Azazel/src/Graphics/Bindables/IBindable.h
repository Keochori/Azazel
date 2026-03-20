#pragma once
#include <d3d11.h>
#include <wrl.h>

using namespace Microsoft::WRL;

template <typename T>
class IBindable
{
public:
	virtual ~IBindable() = default;

	virtual void Create(ComPtr<ID3D11Device>& aDevice) = 0;
	virtual void Bind(ComPtr<ID3D11DeviceContext>& aContext) = 0;

	virtual T* Get() const = 0;
	virtual T* const* GetAddressOf() const = 0;

private:
};

