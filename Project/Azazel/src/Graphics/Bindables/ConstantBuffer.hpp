#pragma once
#include "Graphics/Bindables/IBindable.h"
#include "ConstantBufferStructs.h"
#include <wrl.h>

using namespace Microsoft::WRL;

enum class eBindType
{
	VS,
	PS,
	VSandPS
};

template<typename T>
class ConstantBuffer : public IBindable<ID3D11Buffer>
{
public:
	ConstantBuffer(const eBindType aBindType, const T& aData);
	void Create(DX11& aDX11) override;
	void Bind(DX11& aDX11) override;
	ID3D11Buffer* Get() const override;
	ID3D11Buffer* const* GetAddressOf() const override;

private:
	const T myData;
	const eBindType myBindType;
	ComPtr<ID3D11Buffer> myBuffer;
};

template <typename T>
inline ConstantBuffer<T>::ConstantBuffer(const eBindType aBindType, const T& aData) : myData(aData), myBindType(aBindType) {}

template <typename T>
inline void ConstantBuffer<T>::Create(DX11& aDX11)
{
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc = {};
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0u;
	bufferDesc.ByteWidth = sizeof(myData); // CHANGE
	bufferDesc.StructureByteStride = sizeof(T); // CHANGE

	D3D11_SUBRESOURCE_DATA subResData = {};
	subResData = {};
	subResData.pSysMem = &myData; // CHANGE
	DXASSERT(aDX11.GetDevice()->CreateBuffer(&bufferDesc, &subResData, &myBuffer));
}

template <typename T>
inline void ConstantBuffer<T>::Bind(DX11& aDX11)
{
	switch (myBindType)
	{
		case eBindType::VS:
			aDX11.GetContext()->VSSetConstantBuffers(0u, 1u, myBuffer.GetAddressOf());
			break;

		case eBindType::PS:
			aDX11.GetContext()->PSSetConstantBuffers(0u, 1u, myBuffer.GetAddressOf());
			break;

		case eBindType::VSandPS:
			aDX11.GetContext()->VSSetConstantBuffers(0u, 1u, myBuffer.GetAddressOf());
			aDX11.GetContext()->PSSetConstantBuffers(0u, 1u, myBuffer.GetAddressOf());
			break;
	}
}

template <typename T>
inline ID3D11Buffer* ConstantBuffer<T>::Get() const
{
	return myBuffer.Get();
}

template <typename T>
inline ID3D11Buffer* const* ConstantBuffer<T>::GetAddressOf() const
{
	return myBuffer.GetAddressOf();
}