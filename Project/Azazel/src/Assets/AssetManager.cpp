#include "pch.h"
#include "AssetManager.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "MeshData.h"

std::shared_ptr<Mesh> AssetManager::GetMesh(std::string aMeshName, ComPtr<ID3D11Device>& aDevice)
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

        // Load Vertices
        for (int v = 0; v < currentMesh->mNumVertices; v++)
        {
            auto& vertex = currentMesh->mVertices[v];
            meshData->myVertices.emplace_back(vertex.x, vertex.y, vertex.z);
        }
    }

    return meshData;
}
