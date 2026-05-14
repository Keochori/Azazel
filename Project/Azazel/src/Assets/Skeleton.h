#pragma once
#include "DirectXMath.h"
#include <unordered_map>

struct BoneData
{
	DirectX::XMMATRIX myOffsetMatrix;
	unsigned int myIndex;

	BoneData() = default;
	BoneData(DirectX::XMMATRIX aOffsetMatrix, unsigned int aIndex) : myOffsetMatrix(aOffsetMatrix), myIndex(aIndex) {}
};

struct Skeleton
{
	std::unordered_map<std::string, BoneData> myBoneDataMap;
};