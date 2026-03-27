#pragma once
#include <unordered_map>
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"
#include <memory>

struct MeshData;
struct TextureData;

class AssetManager
{
public:
	AssetManager() = default;
	~AssetManager() = default;

	const std::shared_ptr<Material>& CreateMaterial(const std::string& aName, ComPtr<ID3D11Device>& aDevice);

	const std::shared_ptr<Mesh>& GetMesh(std::string aMeshName, ComPtr<ID3D11Device>& aDevice);
	const std::shared_ptr<Texture>& GetTexture(std::string aTextureName, ComPtr<ID3D11Device>& aDevice);
	const std::shared_ptr<Material>& GetMaterial(std::string aMaterialName);

private:
	std::shared_ptr<MeshData> LoadMeshData(std::string aMeshName);
	std::shared_ptr<TextureData> LoadTextureData(std::string aTextureName);
	std::unordered_map<std::string, std::shared_ptr<Mesh>> myMeshRegistry;
	std::unordered_map<std::string, std::shared_ptr<Texture>> myTextureRegistry;
	std::unordered_map<std::string, std::shared_ptr<Material>> myMaterialRegistry;
};

