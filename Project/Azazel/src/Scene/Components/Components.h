#pragma once
#include "Scene/Types/Transform.h"
#include <memory>

struct Model;
struct Material;

struct TransformComponent
{
	Transform myTransform;
};

struct ModelComponent
{
	std::shared_ptr<Model> myModel;
	std::shared_ptr<Material> myMaterial;
};