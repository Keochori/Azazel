#include "pch.h"
#include "Camera.h"

Camera::Camera()
{
	myPosition = XMVectorZero();
	myRotation = XMFLOAT3(0, 0, 0);

	myForwardWorld = XMVectorSet(0, 0, 1, 0);
	myRightWorld = XMVectorSet(1, 0, 0, 0);
	myUpWorld = XMVectorSet(0, 1, 0, 0);

	myForward = myForwardWorld;
	myRight = myRightWorld;
	myUp = myUpWorld;
}

void Camera::Move(float aX, float aY, float aZ)
{
	myPosition += myForward * aZ;
	myPosition += myRight * aX;
	myPosition += myUp * aY;
}

void Camera::Rotate(float aPitch, float aYaw)
{
	myRotation.x += aPitch;
	myRotation.y += aYaw;

	XMVECTOR pitchQuat = XMQuaternionRotationAxis(myRightWorld, myRotation.x);
	XMVECTOR yawQuat = XMQuaternionRotationAxis(myUpWorld, myRotation.y);

	XMVECTOR quat = XMQuaternionMultiply(pitchQuat, yawQuat);
	quat = XMQuaternionNormalize(quat);

	myForward = XMVector3Rotate(myForwardWorld, quat);
	myRight = XMVector3Rotate(myRightWorld, quat);
	myUp = XMVector3Rotate(myUpWorld, quat);
}

XMMATRIX Camera::GetViewMatrix() const
{
	return XMMatrixLookToLH(myPosition, myForward, myUp);
}
