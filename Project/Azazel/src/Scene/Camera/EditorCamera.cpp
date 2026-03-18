#include "pch.h"
#include "EditorCamera.h"
#include "Tools/Input.h"
#include "Tools/Timer.h"

EditorCamera::EditorCamera(float aStartingSpeed,  float aMinSpeed, float aMaxSpeed, float aRotationSpeed, float aZoomSpeed)
{
	myCurrentSpeed = aStartingSpeed;
	myMinSpeed = aMinSpeed;
	myMaxSpeed = aMaxSpeed;
	myRotationSpeed = aRotationSpeed;
	myZoomSpeed = aZoomSpeed;
}

EditorCamera::~EditorCamera()
{
}

void EditorCamera::Rotate(float aPitch, float aYaw)
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

void EditorCamera::Update()
{
	short wheelDelta = INPUT.GetMouseWheelDelta();

	// Holding right-click
	if (INPUT.IsMouseButtonDown(eKeys::MOUSERBUTTON))
	{
		// Change camera speed with mousewheel
		if (wheelDelta != 0)
			myCurrentSpeed = std::clamp(myCurrentSpeed + wheelDelta * 0.01f, myMinSpeed, myMaxSpeed);

		// Movement
		float movementSpeedDelta = myCurrentSpeed * TIMER.GetDeltaTime();
		if (INPUT.IsKeyDown(eKeys::W))
			Move(0, 0, movementSpeedDelta);
		if (INPUT.IsKeyDown(eKeys::S))
			Move(0, 0, -movementSpeedDelta);
		if (INPUT.IsKeyDown(eKeys::A))
			Move(-movementSpeedDelta, 0, 0);
		if (INPUT.IsKeyDown(eKeys::D))
			Move(movementSpeedDelta, 0, 0);
		if (INPUT.IsKeyDown(eKeys::Q))
			Move(0, -movementSpeedDelta, 0);
		if (INPUT.IsKeyDown(eKeys::E))
			Move(0, movementSpeedDelta, 0);

		// Rotation
		POINT mouseDelta = INPUT.GetMouseDelta();
		if (mouseDelta.x != 0 || mouseDelta.y != 0)
			Rotate(mouseDelta.y * myRotationSpeed * 0.001f, mouseDelta.x * myRotationSpeed * 0.001f);
	}
	else
	{
		if (wheelDelta != 0)
			Move(0, 0, wheelDelta * myZoomSpeed * 0.001f);
	}
}
