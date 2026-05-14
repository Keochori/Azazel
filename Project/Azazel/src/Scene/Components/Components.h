#pragma once
#include "Scene/Types/Transform.h"
#include <memory>

struct Model;
struct Material;
struct Animation;

struct TransformComponent
{
	Transform myTransform;
};

struct ModelComponent
{
	std::shared_ptr<Model> myModel;
	std::shared_ptr<Material> myMaterial;
};

struct AnimationComponent
{
	std::shared_ptr<Animation> myAnimation;
};