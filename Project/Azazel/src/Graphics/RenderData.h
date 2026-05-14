#pragma once
#include "Scene/Types/Transform.h"
#include "DirectXMath.h"

struct Model;
struct Material;

struct RenderData
{
	Transform myTransform;
	std::shared_ptr<Model> myModel;
	std::shared_ptr<Material> myMaterial;
	const std::vector<DirectX::XMMATRIX> myFinalBoneMatrices;

	RenderData(const Transform& aTransform, const std::shared_ptr<Model>& aModel, const std::shared_ptr<Material>& aMaterial,
		const std::vector<DirectX::XMMATRIX> aFinalBoneMatrices) :
		myTransform(aTransform), myModel(aModel), myMaterial(aMaterial), myFinalBoneMatrices(aFinalBoneMatrices) {}
};