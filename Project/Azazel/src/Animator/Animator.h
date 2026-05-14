#pragma once
#include "DirectXMath.h"
#include "Assets/AnimationData.h"
#include "Assets/Skeleton.h"
#include <vector>

struct KeyIndexData
{
	unsigned int myCurrentPositionKeyIndex = 0;
	unsigned int myCurrentRotationKeyIndex = 0;
	unsigned int myCurrentScalingKeyIndex = 0;

	void Reset()
	{
		myCurrentPositionKeyIndex = 0;
		myCurrentRotationKeyIndex = 0;
		myCurrentScalingKeyIndex = 0;
	}
};

struct Animation
{
	bool myPlaying = true;
	bool myLooping = true;
	double mySpeed = 1.0f;

	double myCurrentTimeInSeconds = 0.0;
	double myDurationInSeconds = 0.0;
	KeyIndexData myKeyIndexData;
	std::shared_ptr<Skeleton> mySkeleton;
	std::shared_ptr<AnimationData> myAnimationData;
	std::vector<DirectX::XMMATRIX> myFinalBoneMatrices;

	Animation(const std::shared_ptr<Skeleton>& aSkeleton, const std::shared_ptr<AnimationData>& aAnimationData) 
		: mySkeleton(aSkeleton), myAnimationData(aAnimationData)
	{
		myDurationInSeconds = aAnimationData->myDurationInTicks / aAnimationData->myTicksPerSecond;
		myFinalBoneMatrices.resize(aSkeleton->myBoneDataMap.size());
		for (int i = 0; i < myFinalBoneMatrices.size(); i++)
			myFinalBoneMatrices[i] = DirectX::XMMatrixIdentity();
	}
};

class Animator
{
public:
	Animator() = default;
	~Animator() = default;

	void Update();
	std::shared_ptr<Animation> AddAnimation(const std::shared_ptr<Skeleton>& aSkeleton, const std::shared_ptr<AnimationData>& aAnimationData);

private:
	void TraverseNodeHierarchy(AnimationNode* aNode, DirectX::XMMATRIX aParentTransform, Animation& aAnimation);
	void CalculateLerpedValue(DirectX::XMVECTOR& aVector, unsigned int& aCurrentFrameIndex, double aCurrentTimeInTicks, const std::vector<KeyFrame>& aKeyFrames, bool aQuaternion);
	std::vector<std::shared_ptr<Animation>> myAnimations;
};

