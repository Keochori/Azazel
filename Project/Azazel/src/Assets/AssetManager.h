#pragma once
#include <unordered_map>
#include "Model.h"
#include "Texture.h"
#include "Material.h"
#include <memory>

struct ModelData;
struct TextureData;

class AssetManager
{
public:
	AssetManager() = default;
	~AssetManager() = default;

	const std::shared_ptr<Material>& CreateMaterial(const std::string& aMaterialName, ComPtr<ID3D11Device>& aDevice);

	const std::shared_ptr<Model>& GetModel(std::string aModelName, ComPtr<ID3D11Device>& aDevice);
	const std::shared_ptr<Texture>& GetTexture(std::string aTextureName, ComPtr<ID3D11Device>& aDevice);
	const std::shared_ptr<Material>& GetMaterial(std::string aMaterialName);

private:
	std::shared_ptr<ModelData> LoadModelData(std::string aModelName);
	std::shared_ptr<TextureData> LoadTextureData(std::string aTextureName);
	std::unordered_map<std::string, std::shared_ptr<Model>> myModelRegistry;
	std::unordered_map<std::string, std::shared_ptr<Texture>> myTextureRegistry;
	std::unordered_map<std::string, std::shared_ptr<Material>> myMaterialRegistry;
};

