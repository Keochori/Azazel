#pragma once
#include "Graphics/Bindables/Vertex.h"
#include "Skeleton.h"
#include <vector>
#include <memory>

struct ModelData
{
	std::vector<Vertex> myVertices;
	std::vector<unsigned short> myIndices;
	std::shared_ptr<Skeleton> mySkeleton;
};