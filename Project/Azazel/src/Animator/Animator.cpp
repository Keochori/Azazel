#include "pch.h"
#include "Animator.h"
#include "Tools/Timer.h"
#include "Assets/Skeleton.h"

void Animator::Update()
{
	for (int a = 0; a < myAnimations.size(); a++)
	{
		Animation& animation = *myAnimations[a];
		if (!animation.myPlaying)
			continue;

		if (animation.myCurrentTimeInSeconds < animation.myDurationInSeconds)
		{
			double currentTimeInTicks = animation.myCurrentTimeInSeconds * animation.myAnimationData.myTicksPerSecond;
			TraverseNodeHierarchy(animation.myAnimationData.myRootNode, DirectX::XMMatrixIdentity(), animation);

			animation.myCurrentTimeInSeconds += TIMER.GetDeltaTime() * animation.mySpeed;
		}
		else if (animation.myLooping)
		{
			animation.myCurrentTimeInSeconds = 0.0;
			animation.myKeyIndexData.Reset();
		}
	}
}

std::shared_ptr<Animation> Animator::AddAnimation(std::shared_ptr<Skeleton> aSkeleton, std::shared_ptr<AnimationData> aAnimationData)
{
	if (!aSkeleton)
	{
		LOG_WARNING("Couldn't add animation. Skeleton doesn't exist.");
		return nullptr;
	}
	if (!aAnimationData)
	{
		LOG_WARNING("Couldn't add animation. Animation data doesn't exist.");
		return nullptr;
	}

	std::shared_ptr<Animation> animation = std::make_shared<Animation>(*aSkeleton, *aAnimationData);

	// Animate first frame
	double currentTimeInTicks = animation->myCurrentTimeInSeconds * animation->myAnimationData.myTicksPerSecond;
	TraverseNodeHierarchy(animation->myAnimationData.myRootNode, DirectX::XMMatrixIdentity(), *animation);

	myAnimations.push_back(animation);

	return animation;
}

void Animator::TraverseNodeHierarchy(AnimationNode* aNode, DirectX::XMMATRIX aParentTransform, Animation& aAnimation)
{
	DirectX::XMMATRIX localTransform = aNode->myLocalTransform;

	// Calculate animated transform
	if (aNode->myIsAnimated)
	{
		unsigned int channelIndex = aNode->myChannelIndex;

		DirectX::XMVECTOR positionVector = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		DirectX::XMVECTOR rotationVector = DirectX::XMQuaternionIdentity();
		DirectX::XMVECTOR scalingVector = DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);

		KeyIndexData& keyIndexData = aAnimation.myKeyIndexData;
		double currentTimeInTicks = aAnimation.myCurrentTimeInSeconds * aAnimation.myAnimationData.myTicksPerSecond;
		const std::vector<AnimationChannel>& animationChannels = aAnimation.myAnimationData.myAnimationChannels;

		CalculateLerpedValue(positionVector, keyIndexData.myCurrentPositionKeyIndex, currentTimeInTicks, animationChannels[channelIndex].myPositionKeys, false);
		CalculateLerpedValue(rotationVector, keyIndexData.myCurrentRotationKeyIndex, currentTimeInTicks, animationChannels[channelIndex].myRotationKeys, true);
		CalculateLerpedValue(scalingVector, keyIndexData.myCurrentScalingKeyIndex, currentTimeInTicks, animationChannels[channelIndex].myScalingKeys, false);

		localTransform =
			DirectX::XMMatrixScalingFromVector(scalingVector) *
			DirectX::XMMatrixRotationQuaternion(rotationVector) *
			DirectX::XMMatrixTranslationFromVector(positionVector);
	}

	DirectX::XMMATRIX globalTransform = localTransform * aParentTransform;

	// Update final matrix if node is a bone
	const std::unordered_map<std::string, BoneData>& boneDataMap = aAnimation.mySkeleton.myBoneDataMap;
	if (boneDataMap.contains(aNode->myName))
	{
		const BoneData& boneData = boneDataMap.at(aNode->myName);
		aAnimation.myFinalBoneMatrices[boneData.myIndex] = DirectX::XMMatrixTranspose(boneData.myOffsetMatrix * globalTransform);
	}

	// Continue traversing hierarchy
	for (int c = 0; c < aNode->myChildren.size(); c++)
		TraverseNodeHierarchy(aNode->myChildren[c], globalTransform, aAnimation);
}

void Animator::CalculateLerpedValue(DirectX::XMVECTOR& aVector, unsigned int& aCurrentFrameIndex, double aCurrentTimeInTicks, const std::vector<KeyFrame>& aKeyFrames, bool aQuaternion)
{
	if (!aKeyFrames.empty())
	{
		if (aCurrentFrameIndex + 1 >= aKeyFrames.size())
		{
			aVector = aKeyFrames[aKeyFrames.size() - 1].myValue;
			return;
		}

		if (aCurrentTimeInTicks >= aKeyFrames[aCurrentFrameIndex + 1].myTime)
			aCurrentFrameIndex += 1;

		double currentFrameTime = aKeyFrames[aCurrentFrameIndex].myTime;
		double nextFrameTime = aKeyFrames[aCurrentFrameIndex + 1].myTime;

		double alpha = 0.0;
		if (currentFrameTime != nextFrameTime)
			alpha = (aCurrentTimeInTicks - currentFrameTime) / (nextFrameTime - currentFrameTime);
		alpha = (double)std::clamp((float)alpha, 0.0f, 1.0f);

		if (aQuaternion)
			aVector = DirectX::XMQuaternionNormalize(DirectX::XMQuaternionSlerp(aKeyFrames[aCurrentFrameIndex].myValue, aKeyFrames[aCurrentFrameIndex + 1].myValue, alpha));
		else
			aVector = DirectX::XMVectorLerp(aKeyFrames[aCurrentFrameIndex].myValue, aKeyFrames[aCurrentFrameIndex + 1].myValue, alpha);
		return;
	}
}
