#pragma once
#include "Graphics/Bindables/IBindable.h"
#include "ConstantBufferStructs.h"
#include "Graphics/Diagnostics/DXASSERT.h"

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
	ConstantBuffer(const eBindType& aBindType, unsigned int aBindSlot, const T& aData);
	void Create(ComPtr<ID3D11Device>& aDevice) override;
	void Bind(ComPtr<ID3D11DeviceContext>& aContext) override;
	void UpdateData(ComPtr<ID3D11DeviceContext>& aContext, const T& aData);
	ID3D11Buffer* Get() const override;
	ID3D11Buffer* const* GetAddressOf() const override;

private:
	T myData;
	unsigned int myBindSlot;
	const eBindType myBindType;
	ComPtr<ID3D11Buffer> myBuffer;
};

template <typename T>
inline ConstantBuffer<T>::ConstantBuffer(const eBindType& aBindType, unsigned int aBindSlot, const T& aData)
	: myData(aData), myBindSlot(aBindSlot), myBindType(aBindType) {}

template <typename T>
inline void ConstantBuffer<T>::Create(ComPtr<ID3D11Device>& aDevice)
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
	DXASSERT(aDevice->CreateBuffer(&bufferDesc, &subResData, &myBuffer));
}

template <typename T>
inline void ConstantBuffer<T>::Bind(ComPtr<ID3D11DeviceContext>& aContext)
{
	switch (myBindType)
	{
		case eBindType::VS:
			DXASSERT(aContext->VSSetConstantBuffers(myBindSlot, 1u, myBuffer.GetAddressOf()));
			break;

		case eBindType::PS:
			DXASSERT(aContext->PSSetConstantBuffers(myBindSlot, 1u, myBuffer.GetAddressOf()));
			break;

		case eBindType::VSandPS:
			DXASSERT(aContext->VSSetConstantBuffers(myBindSlot, 1u, myBuffer.GetAddressOf()));
			DXASSERT(aContext->PSSetConstantBuffers(myBindSlot, 1u, myBuffer.GetAddressOf()));
			break;
	}
}

template <typename T>
inline void ConstantBuffer<T>::UpdateData(ComPtr<ID3D11DeviceContext>& aContext, const T& aData)
{
	myData = aData;
	D3D11_MAPPED_SUBRESOURCE mappedSubRes = {};

	HRASSERT(aContext->Map(myBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubRes));
	memcpy(mappedSubRes.pData, &aData, sizeof(T));
	DXASSERT(aContext->Unmap(myBuffer.Get(), 0));
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
