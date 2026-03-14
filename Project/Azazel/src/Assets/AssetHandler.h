#pragma once
#include <unordered_map>
#include "Graphics/Bindables/Vertex.h"

struct Mesh
{
	std::vector<Vertex> myVertices;
	std::vector<unsigned short> myIndices;
};

class AssetHandler
{
public:
	AssetHandler();
	~AssetHandler();

	void LoadMesh(std::string aMeshName);
	const Mesh& GetMesh(std::string aMeshName) const;

private:
	std::unordered_map<std::string, Mesh> myMeshRegistry;
};

