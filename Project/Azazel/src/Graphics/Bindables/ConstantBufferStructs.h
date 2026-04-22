#pragma once
#include <DirectXMath.h>
#include <array>

struct alignas(16) WVPBuffer
{
	DirectX::XMMATRIX myWVP;
	WVPBuffer(const DirectX::XMMATRIX& aMatrix) : myWVP(aMatrix) {}
};

struct alignas(16) MaterialBuffer
{
	int myHasMaterial;
	int aHasAlbedoTexture;
	float padding[2];
	DirectX::XMFLOAT4 myAlbedoColor;

	MaterialBuffer(int aHasMaterial, int aHasAlbedoTexture, const DirectX::XMFLOAT4& aAlbedoColor) :
		myHasMaterial(aHasMaterial), aHasAlbedoTexture(aHasAlbedoTexture), myAlbedoColor(aAlbedoColor) {}
};