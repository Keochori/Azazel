#pragma once
#include "DirectXMath.h"
#include <memory>

class EditorCamera;

class Scene
{
public:
	Scene();
	~Scene();

	void Update();
	DirectX::XMMATRIX GetEditorCameraViewMatrix() const;

private:
	std::unique_ptr<EditorCamera> myEditorCamera;
};

