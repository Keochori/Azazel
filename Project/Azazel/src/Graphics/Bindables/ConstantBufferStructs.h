#pragma once
#include <DirectXMath.h>
#include <array>

struct alignas(16) TransformBuffer
{
	TransformBuffer(const DirectX::XMMATRIX& aMatrix) : myTransform(aMatrix) {}
	DirectX::XMMATRIX myTransform;
};

struct alignas(16) ColorBuffer
{
	ColorBuffer(const std::array<DirectX::XMVECTOR, 6>& aArray) : myFaceColors(aArray) {}
	std::array<DirectX::XMVECTOR, 6> myFaceColors;
};