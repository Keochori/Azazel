#pragma once
#include <DirectXMath.h>
#include <array>

struct alignas(16) TransformBuffer
{
	TransformBuffer(const DirectX::XMMATRIX& aMatrix) : transform(aMatrix) {}
	DirectX::XMMATRIX transform;
};

struct alignas(16) ColorBuffer
{
	ColorBuffer(const std::array<DirectX::XMVECTOR, 6>& aArray) : faceColors(aArray) {}
	std::array<DirectX::XMVECTOR, 6> faceColors;
};