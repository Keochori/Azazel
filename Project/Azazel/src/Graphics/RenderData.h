#pragma once
#include "Scene/Types/Transform.h"

struct Model;
struct Material;

struct RenderData
{
	Transform myTransform;
	std::shared_ptr<Model> myModel;
	std::shared_ptr<Material> myMaterial;

	RenderData(const Transform& aTransform, const std::shared_ptr<Model>& aModel, const std::shared_ptr<Material>& aMaterial) :
		myTransform(aTransform), myModel(aModel), myMaterial(aMaterial) {}
};