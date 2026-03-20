#pragma once
#include "Graphics/Bindables/VertexBuffer.h"
#include "Graphics/Bindables/IndexBuffer.h"

struct Mesh
{
	Mesh(std::vector<Vertex> aVertices, std::vector<unsigned short> aIndices) : myVertexBuffer(aVertices), myIndexBuffer(aIndices) {}
	VertexBuffer myVertexBuffer;
	IndexBuffer myIndexBuffer;
};