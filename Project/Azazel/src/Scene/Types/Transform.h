#pragma once
#include <DirectXMath.h>

using namespace DirectX;

struct Transform
{
	XMFLOAT3 myPosition = XMFLOAT3(0, 0, 0);
	XMFLOAT3 myRotation = XMFLOAT3(0, 0, 0);
	XMFLOAT3 myScale = XMFLOAT3(1.0f, 1.0f, 1.0f);

	void SetPosition(float aX, float aY, float aZ)
	{
		myPosition.x = aX;
		myPosition.y = aY;
		myPosition.z = aZ;
	}

	void SetRotation(float aX, float aY, float aZ)
	{
		myRotation.x = aX;
		myRotation.y = aY;
		myRotation.z = aZ;
	}

	void SetScale(float aX, float aY, float aZ)
	{
		myScale.x = aX;
		myScale.y = aY;
		myScale.z = aZ;
	}
};