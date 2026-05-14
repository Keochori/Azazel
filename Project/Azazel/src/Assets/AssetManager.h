#pragma once
#include "Model.h"
#include "Texture.h"
#include "Material.h"
#include "assimp/matrix4x4.h"
#include "DirectXMath.h"
#include <unordered_map>
#include <memory>

struct ModelData;
struct TextureData;
struct AnimationData;
struct AnimationNode;
struct aiNode;
struct aiScene;
struct SkinningData;
struct BoneData;

class AssetManager
{
public:
	AssetManager() = default;
	~AssetManager() = default;

	std::shared_ptr<Material> CreateMaterial(const std::string& aMaterialName, ComPtr<ID3D11Device>& aDevice);

	std::shared_ptr<Model> GetModel(const std::string& aModelName, ComPtr<ID3D11Device>& aDevice);
	std::shared_ptr<Texture> GetTexture(const std::string& aTextureName, ComPtr<ID3D11Device>& aDevice);
	std::shared_ptr<AnimationData> GetAnimation(const std::string& aAnimationName);
	std::shared_ptr<Material> GetMaterial(const std::string& aMaterialName);

private:
	std::shared_ptr<ModelData> LoadModelData(const std::string& aModelName);
	std::vector<std::vector<SkinningData>> LoadSkinningData(const aiScene* aScene, unsigned int aVertexAmount, const std::unordered_map<std::string, BoneData>& aBoneDataMap);
	void ApplySkinningData(std::vector<std::vector<SkinningData>>& aPerVertexSkinningData, std::vector<Vertex>& aVertexList);
	DirectX::XMMATRIX MatrixAIToXM(const aiMatrix4x4& aAIMatrix);

	std::shared_ptr<TextureData> LoadTextureData(const std::string& aTextureName);
	std::shared_ptr<AnimationData> LoadAnimationData(const std::string& aAnimationName);
	void BuildAnimationNodeHierarchy(aiNode* aNode, AnimationNode* aParentAnimationNode, AnimationData& aAnimationData);
	std::unordered_map<std::string, std::shared_ptr<Model>> myModelRegistry;
	std::unordered_map<std::string, std::shared_ptr<Texture>> myTextureRegistry;
	std::unordered_map<std::string, std::shared_ptr<Material>> myMaterialRegistry;
	std::unordered_map<std::string, std::shared_ptr<AnimationData>> myAnimationRegistry;
};

