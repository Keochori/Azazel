#pragma once
#include "Graphics/Bindables/VertexBuffer.h"
#include "Graphics/Bindables/IndexBuffer.h"
#include "Skeleton.h"

struct Model
{
	VertexBuffer myVertexBuffer;
	IndexBuffer myIndexBuffer;
	std::shared_ptr<Skeleton> mySkeleton;

	Model(const std::vector<Vertex>& aVertices, const std::vector<unsigned short>& aIndices, const std::shared_ptr<Skeleton>& aSkeleton)
		: myVertexBuffer(aVertices), myIndexBuffer(aIndices), mySkeleton(aSkeleton) {}
};