#pragma once
#include "Graphics/DX11.h"

template <typename T>
class IBindable
{
public:
	virtual ~IBindable() = default;

	virtual void Create(DX11& aDX11) = 0;
	virtual void Bind(DX11& aDX11) = 0;

	virtual T* Get() const = 0;
	virtual T* const* GetAddressOf() const = 0;

private:
};

