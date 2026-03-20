#pragma once
#include <unordered_map>
#include "Mesh.h"
#include <memory>

struct MeshData;

class AssetManager
{
public:
	AssetManager() = default;
	~AssetManager() = default;

	std::shared_ptr<Mesh> GetMesh(std::string aMeshName, ComPtr<ID3D11Device>& aDevice);

private:
	std::shared_ptr<MeshData> LoadMeshData(std::string aMeshName);
	std::unordered_map<std::string, std::shared_ptr<Mesh>> myMeshRegistry;
};

