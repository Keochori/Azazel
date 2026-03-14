#include "pch.h"
#include "AssetHandler.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

AssetHandler::AssetHandler()
{
}

AssetHandler::~AssetHandler()
{
}

void AssetHandler::LoadMesh(std::string aMeshName)
{
	Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile("resources/meshes/" + aMeshName,
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType);

    if (nullptr == scene)
    {
        LOG_ERROR(importer.GetErrorString());
        return;
    }

    Mesh meshData;
    for (int m = 0; m < scene->mNumMeshes; m++)
    {
        auto& currentMesh = scene->mMeshes[m];

        // Load Indices
        int indexOffset = meshData.myVertices.size();
        for (int f = 0; f < currentMesh->mNumFaces; f++)
        {
            auto& face = currentMesh->mFaces[f];
            for (int i = 0; i < face.mNumIndices; i++)
                meshData.myIndices.emplace_back(face.mIndices[i] + indexOffset);
        }

        // Load Vertices
        for (int v = 0; v < currentMesh->mNumVertices; v++)
        {
            auto& vertex = currentMesh->mVertices[v];
            meshData.myVertices.emplace_back(vertex.x, vertex.y, vertex.z);
        }
    }

    myMeshRegistry.try_emplace(aMeshName, meshData);
}

const Mesh& AssetHandler::GetMesh(std::string aMeshName) const
{
    if (myMeshRegistry.find(aMeshName) == myMeshRegistry.end())
    {
        LOG_ERROR("Mesh '" + aMeshName + "' not found in registry.");
        ASSERT(false);
    }

    return myMeshRegistry.at(aMeshName);
}
