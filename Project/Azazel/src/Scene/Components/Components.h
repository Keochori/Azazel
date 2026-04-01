#pragma once
#include "Scene/Types/Transform.h"
#include <memory>

struct Mesh;
struct Material;

struct TransformComponent
{
	Transform myTransform;
};

struct MeshComponent
{
	std::shared_ptr<Mesh> myMesh;
	std::shared_ptr<Material> myMaterial;
};