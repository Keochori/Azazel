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
			double currentTimeInTicks = animation.myCurrentTimeInSeconds * animation.myAnimationData->myTicksPerSecond;
			TraverseNodeHierarchy(animation.myAnimationData->myRootNode, DirectX::XMMatrixIdentity(), currentTimeInTicks,
				*animation.mySkeleton, animation.myAnimationData->myAnimationChannels, animation.myFinalBoneMatrices);

			animation.myCurrentTimeInSeconds += TIMER.GetDeltaTime() * animation.mySpeed;
		}
		else if (animation.myLooping)
			animation.myCurrentTimeInSeconds = 0.0;
	}

}

std::shared_ptr<Animation> Animator::AddAnimation(const std::shared_ptr<Skeleton>& aSkeleton, const std::shared_ptr<AnimationData>& aAnimationData)
{
	std::shared_ptr<Animation> animation = std::make_shared<Animation>(aSkeleton, aAnimationData);

	// Animate first frame
	double currentTimeInTicks = animation->myCurrentTimeInSeconds * animation->myAnimationData->myTicksPerSecond;
	TraverseNodeHierarchy(animation->myAnimationData->myRootNode, DirectX::XMMatrixIdentity(), currentTimeInTicks,
		*animation->mySkeleton, animation->myAnimationData->myAnimationChannels, animation->myFinalBoneMatrices);

	myAnimations.push_back(animation);

	return animation;
}

void Animator::TraverseNodeHierarchy(AnimationNode* aNode, DirectX::XMMATRIX aParentTransform, double aCurrentTimeInTicks, const Skeleton& aSkeleton, const std::vector<AnimationChannel>& aAnimationChannels, std::vector<DirectX::XMMATRIX>& aFinalBoneMatrices)
{
	DirectX::XMMATRIX localTransform = aNode->myLocalTransform;

	// Check if node is animated
	int channelIndex = -1;
	for (int i = 0; i < aAnimationChannels.size(); i++)
		if (aAnimationChannels[i].myNodeName == aNode->myName)
			channelIndex = i;

	// Calculate animated transform
	if (channelIndex != -1)
	{
		DirectX::XMVECTOR positionVector = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		DirectX::XMVECTOR rotationVector = DirectX::XMQuaternionIdentity();
		DirectX::XMVECTOR scalingVector = DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);

		CalculateLerpedValue(positionVector, aCurrentTimeInTicks, aAnimationChannels[channelIndex].myPositionKeys, false);
		CalculateLerpedValue(rotationVector, aCurrentTimeInTicks, aAnimationChannels[channelIndex].myRotationKeys, true);
		CalculateLerpedValue(scalingVector, aCurrentTimeInTicks, aAnimationChannels[channelIndex].myScalingKeys, false);

		localTransform =
			DirectX::XMMatrixScalingFromVector(scalingVector) *
			DirectX::XMMatrixRotationQuaternion(rotationVector) *
			DirectX::XMMatrixTranslationFromVector(positionVector);
	}

	DirectX::XMMATRIX globalTransform = localTransform * aParentTransform;

	// Update final matrix if node is a bone
	std::unordered_map<std::string, BoneData> boneDataMap = aSkeleton.myBoneDataMap;
	if (boneDataMap.contains(aNode->myName))
	{
		BoneData& boneData = boneDataMap[aNode->myName];
		aFinalBoneMatrices[boneData.myIndex] = DirectX::XMMatrixTranspose(boneData.myOffsetMatrix * globalTransform);
	}

	// Continue traversing hierarchy
	for (int c = 0; c < aNode->myChildren.size(); c++)
		TraverseNodeHierarchy(aNode->myChildren[c], globalTransform, aCurrentTimeInTicks, aSkeleton, aAnimationChannels, aFinalBoneMatrices);
}

void Animator::CalculateLerpedValue(DirectX::XMVECTOR& aVector, double aCurrentTimeInTicks, const std::vector<KeyFrame>& aKeyFrames, bool aQuaternion)
{
	if (!aKeyFrames.empty())
	{
		// Search for current KeyFrame
		for (int i = 0; i < aKeyFrames.size(); i++)
		{
			// If last or only keyframe, apply its' value
			if (i == aKeyFrames.size() - 1)
			{
				aVector = aKeyFrames[aKeyFrames.size() - 1].myValue;
				return;
			}

			double currentFrameTime = aKeyFrames[i].myTime;
			double nextFrameTime = aKeyFrames[i + 1].myTime;

			// If current KeyFrame, calculate lerped value
			if (aCurrentTimeInTicks >= currentFrameTime && aCurrentTimeInTicks < nextFrameTime)
			{
				double alpha = 0.0;
				if (currentFrameTime != nextFrameTime)
					alpha = (aCurrentTimeInTicks - currentFrameTime) / (nextFrameTime - currentFrameTime);
				alpha = (double)std::clamp((float)alpha, 0.0f, 1.0f);

				if (aQuaternion)
					aVector = DirectX::XMQuaternionNormalize(DirectX::XMQuaternionSlerp(aKeyFrames[i].myValue, aKeyFrames[i + 1].myValue, alpha));
				else
					aVector = DirectX::XMVectorLerp(aKeyFrames[i].myValue, aKeyFrames[i + 1].myValue, alpha);
				return;
			}
		}
	}
}
