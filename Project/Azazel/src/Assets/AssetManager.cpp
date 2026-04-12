#include "pch.h"
#include "AssetManager.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "ModelData.h"
#include "TextureData.h"
#include "Graphics/Diagnostics/DXASSERT.h"

const std::shared_ptr<Material>& AssetManager::CreateMaterial(const std::string& aMaterialName, ComPtr<ID3D11Device>& aDevice)
{
    if (!myMaterialRegistry.contains(aMaterialName))
    {
        Sampler sampler;
        sampler.Create(aDevice);

        std::shared_ptr<Material> material = std::make_shared<Material>(sampler);

        myMaterialRegistry.emplace(aMaterialName, material);
        return material;
    }
    else
    {
        LOG_WARNING("Material '" + aMaterialName + "' already exists.");
        return myMaterialRegistry.at(aMaterialName);
    }
}

const std::shared_ptr<Model>& AssetManager::GetModel(std::string aModelName, ComPtr<ID3D11Device>& aDevice)
{
	if (!myModelRegistry.contains(aModelName))
	{
        std::shared_ptr<ModelData> modelData = LoadModelData(aModelName);
        if (!modelData)
        {
            LOG_WARNING("Unsuccessful attempt to load '" + aModelName + "' model.");
            return nullptr;
        }

        std::shared_ptr<Model> model = std::make_shared<Model>(modelData->myVertices, modelData->myIndices);
        model->myVertexBuffer.Create(aDevice);
        model->myIndexBuffer.Create(aDevice);
        myModelRegistry.emplace(aModelName, model);
        LOG_SUCCESS("Successfully loaded '" + aModelName + "' model.");
	}

	return myModelRegistry.at(aModelName);
}

const std::shared_ptr<Texture>& AssetManager::GetTexture(std::string aTextureName, ComPtr<ID3D11Device>& aDevice)
{
    if (!myTextureRegistry.contains(aTextureName))
    {
        std::shared_ptr<TextureData> textureData = LoadTextureData(aTextureName);
        if (!textureData)
        {
            LOG_WARNING("Unsuccessful attempt to load '" + aTextureName + "' texture.");
            return nullptr;
        }

        std::shared_ptr<Texture> texture = std::make_shared<Texture>(std::move(textureData->myImage));
        texture->mySRV.Create(aDevice);
        myTextureRegistry.emplace(aTextureName, texture);
        LOG_SUCCESS("Successfully loaded '" + aTextureName + "' texture.");
    }

    return myTextureRegistry.at(aTextureName);
}

const std::shared_ptr<Material>& AssetManager::GetMaterial(std::string aMaterialName)
{
    if (!myMaterialRegistry.contains(aMaterialName)) 
    {
        LOG_WARNING("Failed to fetch material. Material '" + aMaterialName + "' doesn't exist in the registry.");
        return nullptr;
    }

    return myMaterialRegistry.at(aMaterialName);
}

std::shared_ptr<ModelData> AssetManager::LoadModelData(std::string aModelName)
{
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile("resources/models/" + aModelName,
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType|
        aiProcess_ConvertToLeftHanded);

    if (nullptr == scene)
    {
        LOG_ERROR(importer.GetErrorString());
        return nullptr;
    }

    std::shared_ptr<ModelData> modelData = std::make_shared<ModelData>();
    // Loop through every sub-mesh
    for (int m = 0; m < scene->mNumMeshes; m++)
    {
        auto& currentMesh = scene->mMeshes[m];

        // Load Indices
        int indexOffset = modelData->myVertices.size();
        for (int f = 0; f < currentMesh->mNumFaces; f++)
        {
            auto& face = currentMesh->mFaces[f];
            for (int i = 0; i < face.mNumIndices; i++)
                modelData->myIndices.emplace_back(face.mIndices[i] + indexOffset);
        }

        // Load Vertices & UV
        for (int v = 0; v < currentMesh->mNumVertices; v++)
        {
            aiVector3D vertex = currentMesh->mVertices[v];
            aiVector3D* albedoTexCoords = currentMesh->mTextureCoords[0];

            if (albedoTexCoords)
                modelData->myVertices.emplace_back(vertex.x, vertex.y, vertex.z, albedoTexCoords[v].x, albedoTexCoords[v].y);
            else
                modelData->myVertices.emplace_back(vertex.x, vertex.y, vertex.z);
        }
    }

    return modelData;
}

std::shared_ptr<TextureData> AssetManager::LoadTextureData(std::string aTextureName)
{
    DirectX::ScratchImage image = DirectX::ScratchImage{};
    std::string texturePath = "resources/textures/" + aTextureName;
    HRASSERT(DirectX::LoadFromWICFile(std::wstring(texturePath.begin(), texturePath.end()).c_str(), DirectX::WIC_FLAGS_NONE, nullptr, image));

    if (image.GetImageCount() == 0)
        return nullptr;

    std::shared_ptr<TextureData> textureData = std::make_shared<TextureData>();
    textureData->myImage = std::move(image);

    return textureData;
}
