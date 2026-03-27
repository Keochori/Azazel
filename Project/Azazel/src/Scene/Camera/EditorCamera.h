#pragma once
#include "Camera.h"

class EditorCamera : public Camera
{
public:
	EditorCamera(float aStartingSpeed, float aMinSpeed, float aMaxSpeed, float aRotationSpeed, float aZoomSpeed);

	void Rotate(float aPitch, float aYaw) override;
	void Update();

private:
	float myCurrentSpeed;
	float myMaxSpeed;
	float myMinSpeed;
	float myRotationSpeed;
	float myZoomSpeed;
};

