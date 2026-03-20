#pragma once
#include "Graphics/Bindables/Vertex.h"
#include <vector>

struct MeshData
{
	std::vector<Vertex> myVertices;
	std::vector<unsigned short> myIndices;
};