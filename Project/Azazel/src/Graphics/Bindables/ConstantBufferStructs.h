#pragma once
#include <DirectXMath.h>
#include <array>

struct alignas(16) WVPBuffer
{
	WVPBuffer(const DirectX::XMMATRIX& aMatrix) : myWVP(aMatrix) {}
	DirectX::XMMATRIX myWVP;
};

struct alignas(16) ColorBuffer
{
	ColorBuffer(const std::array<DirectX::XMVECTOR, 6>& aArray) : myFaceColors(aArray) {}
	std::array<DirectX::XMVECTOR, 6> myFaceColors;
};