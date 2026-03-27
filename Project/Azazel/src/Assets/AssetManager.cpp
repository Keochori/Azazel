#include "pch.h"
#include "AssetManager.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "MeshData.h"
#include "TextureData.h"
#include "Graphics/Diagnostics/DXASSERT.h"

const std::shared_ptr<Material>& AssetManager::CreateMaterial(const std::string& aName, ComPtr<ID3D11Device>& aDevice)
{
    Sampler sampler;
    sampler.Create(aDevice);

    std::shared_ptr<Material> material = std::make_shared<Material>(sampler);

    myMaterialRegistry.emplace(aName, material);
    return material;
}

const std::shared_ptr<Mesh>& AssetManager::GetMesh(std::string aMeshName, ComPtr<ID3D11Device>& aDevice)
{
	if (myMeshRegistry.find(aMeshName) == myMeshRegistry.end())
	{
        std::shared_ptr<MeshData> meshData = LoadMeshData(aMeshName);
        if (!meshData)
        {
            LOG_WARNING("Unsuccessful attempt to load '" + aMeshName + "' mesh.");
            return nullptr;
        }

        std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(meshData->myVertices, meshData->myIndices);
        mesh->myVertexBuffer.Create(aDevice);
        mesh->myIndexBuffer.Create(aDevice);
        myMeshRegistry.emplace(aMeshName, mesh);
        LOG_SUCCESS("Successfully loaded '" + aMeshName + "' mesh.");
	}

	return myMeshRegistry.at(aMeshName);
}

const std::shared_ptr<Texture>& AssetManager::GetTexture(std::string aTextureName, ComPtr<ID3D11Device>& aDevice)
{
    if (myTextureRegistry.find(aTextureName) == myTextureRegistry.end())
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
    if (myMaterialRegistry.find(aMaterialName) == myMaterialRegistry.end())
    {
        LOG_WARNING("Failed to fetch material. Material '" + aMaterialName + "' doesn't exist in the registry.");
        return nullptr;
    }

    return myMaterialRegistry.at(aMaterialName);
}

std::shared_ptr<MeshData> AssetManager::LoadMeshData(std::string aMeshName)
{
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile("resources/meshes/" + aMeshName,
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

    std::shared_ptr<MeshData> meshData = std::make_shared<MeshData>();
    // Loop through every sub-mesh
    for (int m = 0; m < scene->mNumMeshes; m++)
    {
        auto& currentMesh = scene->mMeshes[m];

        // Load Indices
        int indexOffset = meshData->myVertices.size();
        for (int f = 0; f < currentMesh->mNumFaces; f++)
        {
            auto& face = currentMesh->mFaces[f];
            for (int i = 0; i < face.mNumIndices; i++)
                meshData->myIndices.emplace_back(face.mIndices[i] + indexOffset);
        }

        // Load Vertices & UV
        for (int v = 0; v < currentMesh->mNumVertices; v++)
        {
            aiVector3D vertex = currentMesh->mVertices[v];
            aiVector3D* albedoTexCoords = currentMesh->mTextureCoords[0];

            if (albedoTexCoords)
                meshData->myVertices.emplace_back(vertex.x, vertex.y, vertex.z, albedoTexCoords[v].x, albedoTexCoords[v].y);
            else
                meshData->myVertices.emplace_back(vertex.x, vertex.y, vertex.z);
        }
    }

    return meshData;
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
