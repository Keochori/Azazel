#pragma once
#include "DirectXMath.h"
#include "entt/entt.hpp"
#include <memory>

class EditorCamera;
struct RenderData;
class Entity;

class Scene
{
public:
	Scene();
	~Scene();

	void Update();

	Entity& CreateEntity();
	const std::vector<RenderData>& GetRenderData();
	const DirectX::XMMATRIX& GetEditorCameraViewMatrix();

private:
	entt::registry myRegistry;
	std::vector<Entity> myEntities;
	std::unique_ptr<EditorCamera> myEditorCamera;

	std::vector<RenderData> myRenderData;
};

