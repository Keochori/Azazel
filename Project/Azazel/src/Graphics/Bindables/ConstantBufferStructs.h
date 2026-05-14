#pragma once
#include <DirectXMath.h>
#include <array>
#include <vector>

#define MAX_BONES 128 // When changing this number, apply changes to vertex shader accordingly

struct alignas(16) WVPBuffer
{
	DirectX::XMMATRIX myWVP;
	WVPBuffer(const DirectX::XMMATRIX& aMatrix) : myWVP(aMatrix) {}
};

struct alignas(16) MaterialBuffer
{
	int myHasMaterial;
	int myHasAlbedoTexture;
	float myPadding[2];
	DirectX::XMFLOAT4 myAlbedoColor;

	MaterialBuffer(int aHasMaterial, int aHasAlbedoTexture, const DirectX::XMFLOAT4& aAlbedoColor) :
		myHasMaterial(aHasMaterial), myHasAlbedoTexture(aHasAlbedoTexture), myAlbedoColor(aAlbedoColor) {}
};

struct alignas(16) BoneBuffer
{
	int myHasSkeleton;
	float myPadding[3];
	DirectX::XMMATRIX myFinalBoneMatrices[MAX_BONES];

	BoneBuffer(int aHasSkeleton, const std::vector<DirectX::XMMATRIX>& aFinalBoneMatrices) : myHasSkeleton(aHasSkeleton)
	{
		if (aFinalBoneMatrices.size() > MAX_BONES)
			LOG_WARNING("Max animation bone amount reached and clamped for a model.");

		for (int i = 0; i < aFinalBoneMatrices.size(); i++)
			myFinalBoneMatrices[i] = aFinalBoneMatrices[i];
	}
};