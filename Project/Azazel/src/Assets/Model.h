#pragma once
#include "Graphics/Bindables/VertexBuffer.h"
#include "Graphics/Bindables/IndexBuffer.h"

struct Model
{
	Model(const std::vector<Vertex>& aVertices, const std::vector<unsigned short>& aIndices) : myVertexBuffer(aVertices), myIndexBuffer(aIndices) {}
	VertexBuffer myVertexBuffer;
	IndexBuffer myIndexBuffer;
};