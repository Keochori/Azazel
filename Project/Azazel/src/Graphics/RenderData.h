#pragma once
#include "Scene/Types/Transform.h"

struct Mesh;
struct Material;

struct RenderData
{
	Transform myTransform;
	std::shared_ptr<Mesh> myMesh;
	std::shared_ptr<Material> myMaterial;

	RenderData(const Transform& aTransform, const std::shared_ptr<Mesh>& aMesh, const std::shared_ptr<Material>& aMaterial) :
		myTransform(aTransform), myMesh(aMesh), myMaterial(aMaterial) {}
};