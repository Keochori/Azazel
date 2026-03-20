#pragma once
#include "DirectXMath.h"
#include <memory>

class EditorCamera;
class Object;

class Scene
{
public:
	Scene();
	~Scene();

	void Update();
	void AddObject(std::shared_ptr<Object> aObject);
	std::vector<std::shared_ptr<Object>> GetObjects() const;
	DirectX::XMMATRIX GetEditorCameraViewMatrix() const;

private:
	std::unique_ptr<EditorCamera> myEditorCamera;
	std::vector<std::shared_ptr<Object>> myObjects;
};

