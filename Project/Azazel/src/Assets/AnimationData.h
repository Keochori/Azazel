#pragma once
#include "DirectXMath.h"
#include <vector>

struct KeyFrame
{
	DirectX::XMVECTOR myValue;
	double myTime;

	KeyFrame(double aTime, DirectX::XMVECTOR aValue) : myTime(aTime), myValue(aValue) {}
};

struct AnimationChannel
{
	std::string myNodeName;

	std::vector<KeyFrame> myPositionKeys;
	std::vector<KeyFrame> myRotationKeys;
	std::vector<KeyFrame> myScalingKeys;
};

struct AnimationNode
{
	std::string myName;

	DirectX::XMMATRIX myLocalTransform;
	std::vector<AnimationNode*> myChildren;
};

struct AnimationData
{
	double myDurationInTicks;
	double myTicksPerSecond;
	AnimationNode* myRootNode;
	std::vector<AnimationChannel> myAnimationChannels;
};