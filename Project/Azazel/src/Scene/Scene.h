#pragma once
#include "DirectXMath.h"
#include <memory>

#undef GetObject

class EditorCamera;
class Object;

class Scene
{
public:
	Scene();
	~Scene();

	void Update();
	void AddObject(std::shared_ptr<Object> aObject);

	const std::shared_ptr<Object>& GetObject(const std::string& aName);
	const std::vector<std::shared_ptr<Object>>& GetObjects();
	const DirectX::XMMATRIX& GetEditorCameraViewMatrix();

private:
	std::unique_ptr<EditorCamera> myEditorCamera;
	std::vector<std::shared_ptr<Object>> myObjects;
};

