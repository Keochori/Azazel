#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class Camera
{
public:
	Camera();
	virtual ~Camera();

	void Move(float aX, float aY, float aZ);
	virtual void Rotate(float aPitch, float aYaw);
	XMMATRIX GetViewMatrix() const;

protected:
	XMVECTOR myPosition;
	XMFLOAT3 myRotation;

	XMVECTOR myForward;
	XMVECTOR myRight;
	XMVECTOR myUp;

	XMVECTOR myForwardWorld;
	XMVECTOR myRightWorld;
	XMVECTOR myUpWorld;
};

