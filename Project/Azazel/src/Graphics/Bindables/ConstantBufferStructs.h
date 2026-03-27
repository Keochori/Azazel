#pragma once
#include <DirectXMath.h>
#include <array>

struct alignas(16) WVPBuffer
{
	WVPBuffer(const DirectX::XMMATRIX& aMatrix) : myWVP(aMatrix) {}
	DirectX::XMMATRIX myWVP;
};

struct alignas(16) MaterialBuffer
{
	MaterialBuffer(int aHasMaterial, int aHasAlbedoTexture, const DirectX::XMFLOAT4& aAlbedoColor) :
		myHasMaterial(aHasMaterial), aHasAlbedoTexture(aHasAlbedoTexture), myAlbedoColor(aAlbedoColor) {}
	int myHasMaterial;
	int aHasAlbedoTexture;
	float padding[2];
	DirectX::XMFLOAT4 myAlbedoColor;
};