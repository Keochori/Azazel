#include "pch.h"
#include "AssetManager.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "ModelData.h"
#include "TextureData.h"
#include "SkinningData.h"
#include "AnimationData.h"
#include "Graphics/Diagnostics/DXASSERT.h"

std::shared_ptr<Material> AssetManager::CreateMaterial(const std::string& aMaterialName, ComPtr<ID3D11Device>& aDevice)
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

std::shared_ptr<Model> AssetManager::GetModel(std::string aModelName, ComPtr<ID3D11Device>& aDevice)
{
	if (!myModelRegistry.contains(aModelName))
	{
        std::shared_ptr<ModelData> modelData = LoadModelData(aModelName);
        if (!modelData)
        {
            LOG_WARNING("Unsuccessful attempt to load '" + aModelName + "' model.");
            return nullptr;
        }

        std::shared_ptr<Model> model = std::make_shared<Model>(modelData->myVertices, modelData->myIndices, modelData->mySkeleton);
        model->myVertexBuffer.Create(aDevice);
        model->myIndexBuffer.Create(aDevice);
        myModelRegistry.emplace(aModelName, model);
        LOG_SUCCESS("Successfully loaded '" + aModelName + "' model.");
	}

	return myModelRegistry.at(aModelName);
}

std::shared_ptr<Texture> AssetManager::GetTexture(std::string aTextureName, ComPtr<ID3D11Device>& aDevice)
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

std::shared_ptr<AnimationData> AssetManager::GetAnimation(std::string aAnimationName)
{
    if (!myAnimationRegistry.contains(aAnimationName))
    {
        std::shared_ptr<AnimationData> animationData = LoadAnimationData(aAnimationName);
        if (!animationData)
        {
            LOG_WARNING("Unsuccessful attempt to load '" + aAnimationName + "' animation.");
            return nullptr;
        }

        myAnimationRegistry.emplace(aAnimationName, animationData);
        LOG_SUCCESS("Successfully loaded '" + aAnimationName + "' animation.");
    }

    return myAnimationRegistry.at(aAnimationName);
}

std::shared_ptr<Material> AssetManager::GetMaterial(std::string aMaterialName)
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
    std::unordered_map<std::string, BoneData> boneDataMap;
    // Loop through every sub-mesh
    for (int m = 0; m < scene->mNumMeshes; m++)
    {
        auto& mesh = scene->mMeshes[m];

        // Load Indices
        int vertexIndexOffset = modelData->myVertices.size();
        int boneIndexOffset = boneDataMap.size();
        for (int f = 0; f < mesh->mNumFaces; f++)
        {
            auto& face = mesh->mFaces[f];
            for (int i = 0; i < face.mNumIndices; i++)
                modelData->myIndices.emplace_back(face.mIndices[i] + vertexIndexOffset);
        }

        // Load Vertices & UV
        for (int v = 0; v < mesh->mNumVertices; v++)
        {
            aiVector3D vertex = mesh->mVertices[v];
            aiVector3D* albedoTexCoords = mesh->mTextureCoords[0];

            if (albedoTexCoords)
                modelData->myVertices.emplace_back(vertex.x, vertex.y, vertex.z, albedoTexCoords[v].x, albedoTexCoords[v].y);
            else
                modelData->myVertices.emplace_back(vertex.x, vertex.y, vertex.z);
        }

        // Load Bone Offset Matrices
        for (int b = 0; b < mesh->mNumBones; b++)
        {
            auto& bone = mesh->mBones[b];
            boneDataMap.emplace(bone->mName.C_Str(), BoneData(MatrixAIToXM(bone->mOffsetMatrix), b + boneIndexOffset));
        }
    }

    // Create Skeleton, Load & Apply Skinning Data
    if (!boneDataMap.empty())
    {
        modelData->mySkeleton = std::make_shared<Skeleton>();
        modelData->mySkeleton->myBoneDataMap = boneDataMap;
        auto perVertexSkinningData = LoadSkinningData(scene, modelData->myVertices.size(), modelData->mySkeleton->myBoneDataMap);
        ApplySkinningData(perVertexSkinningData, modelData->myVertices);
    }

    return modelData;
}

std::vector<std::vector<SkinningData>> AssetManager::LoadSkinningData(const aiScene* aScene, unsigned int aVertexAmount, const std::unordered_map<std::string, BoneData>& aBoneDataMap)
{
    std::vector<std::vector<SkinningData>> perVertexSkinningDataList;
    perVertexSkinningDataList.resize(aVertexAmount);
    int vertexIndexOffset = 0;

    // Loop through every sub-mesh
    for (int m = 0; m < aScene->mNumMeshes; m++)
    {
        auto& mesh = aScene->mMeshes[m];

        // Loop through every bone
        for (int b = 0; b < mesh->mNumBones; b++)
        {
            auto& bone = mesh->mBones[b];

            // Load Skinning Data
            for (int w = 0; w < bone->mNumWeights; w++)
            {
                auto& weight = bone->mWeights[w];

                SkinningData skinningData;
                skinningData.myBoneID = aBoneDataMap.at(bone->mName.C_Str()).myIndex;
                skinningData.myWeight = weight.mWeight;
                perVertexSkinningDataList[weight.mVertexId + vertexIndexOffset].push_back(skinningData);
            }
        }
        vertexIndexOffset += mesh->mNumVertices;
    }

    return perVertexSkinningDataList;
}

void AssetManager::ApplySkinningData(std::vector<std::vector<SkinningData>>& aPerVertexSkinningData, std::vector<Vertex>& aVertexList)
{
    for (int v = 0; v < aPerVertexSkinningData.size(); v++)
    {
        auto& skinningDataList = aPerVertexSkinningData[v];

        // Sort by weight influence
        std::sort(skinningDataList.begin(), skinningDataList.end(), [&](const SkinningData& aSD1, const SkinningData& aSD2)
            { return aSD1.myWeight > aSD2.myWeight; });

        // Clamp size
        if (skinningDataList.size() > MAX_BONES_PER_VERTEX)
            skinningDataList.resize(MAX_BONES_PER_VERTEX);

        // Normalize weights
        float sum = 0.0f;
        for (auto& skinningData : skinningDataList)
            sum += skinningData.myWeight;
        if (sum > 0.0f)
        {
            float sumInverse = 1.0f / sum;
            for (auto& skinningData : skinningDataList)
                skinningData.myWeight *= sumInverse;
        }

        // Apply data
        for (int i = 0; i < skinningDataList.size(); i++)
        {
            Vertex& vertex = aVertexList[v];
            vertex.myBoneIDs[i] = skinningDataList[i].myBoneID;
            vertex.myWeights[i] = skinningDataList[i].myWeight;
        }
    }
}

DirectX::XMMATRIX AssetManager::MatrixAIToXM(const aiMatrix4x4& aAIMatrix)
{
    // Transposed from column major to row major
    return DirectX::XMMatrixSet(
        aAIMatrix.a1, aAIMatrix.b1, aAIMatrix.c1, aAIMatrix.d1,
        aAIMatrix.a2, aAIMatrix.b2, aAIMatrix.c2, aAIMatrix.d2,
        aAIMatrix.a3, aAIMatrix.b3, aAIMatrix.c3, aAIMatrix.d3,
        aAIMatrix.a4, aAIMatrix.b4, aAIMatrix.c4, aAIMatrix.d4
    );
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

std::shared_ptr<AnimationData> AssetManager::LoadAnimationData(std::string aAnimationName)
{
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile("resources/animations/" + aAnimationName, aiProcess_ConvertToLeftHanded);

    if (nullptr == scene)
    {
        LOG_ERROR(importer.GetErrorString());
        return nullptr;
    }

    if (scene->mNumAnimations == 0)
    {
        LOG_ERROR("File doesn't have any animations.");
        return nullptr;
    }

    std::shared_ptr<AnimationData> animationData = std::make_shared<AnimationData>();

    // Load Animation Channels
    auto& animation = scene->mAnimations[0];
    animationData->myDurationInTicks = animation->mDuration;
    animationData->myTicksPerSecond = animation->mTicksPerSecond;

    for (int c = 0; c < animation->mNumChannels; c++)
    {
        auto& currentChannel = animation->mChannels[c];
        
        AnimationChannel animationChannel;
        animationChannel.myNodeName = currentChannel->mNodeName.C_Str();

        // Position Keys
        for (int i = 0; i < currentChannel->mNumPositionKeys; i++)
        {
            auto& positionKey = currentChannel->mPositionKeys[i];
            auto& pos = positionKey.mValue;
            animationChannel.myPositionKeys.emplace_back(positionKey.mTime, DirectX::XMVectorSet(pos.x, pos.y, pos.z, 1.0f));
        }

        // Rotation Keys
        for (int i = 0; i < currentChannel->mNumRotationKeys; i++)
        {
            auto& rotationKey = currentChannel->mRotationKeys[i];
            auto& rot = rotationKey.mValue;
            animationChannel.myRotationKeys.emplace_back(rotationKey.mTime, DirectX::XMVectorSet(rot.x, rot.y, rot.z, rot.w));
        }

        // Scaling Keys
        for (int i = 0; i < currentChannel->mNumScalingKeys; i++)
        {
            auto& scalingKey = currentChannel->mScalingKeys[i];
            auto& scale = scalingKey.mValue;
            animationChannel.myScalingKeys.emplace_back(scalingKey.mTime, DirectX::XMVectorSet(scale.x, scale.y, scale.z, 1.0f));
        }

        animationData->myAnimationChannels.push_back(animationChannel);
    }

    // Build Animation Node Hierarchy
    BuildAnimationNodeHierarchy(scene->mRootNode, nullptr, *animationData);

    return animationData;
}

void AssetManager::BuildAnimationNodeHierarchy(aiNode* aNode, AnimationNode* aParentAnimationNode, AnimationData& aAnimationData)
{
    AnimationNode* animationNode = new AnimationNode;
    animationNode->myName = aNode->mName.C_Str();
    animationNode->myLocalTransform = MatrixAIToXM(aNode->mTransformation);

    // Check if node is animated
    std::vector<AnimationChannel>& animationChannels = aAnimationData.myAnimationChannels;
    for (int i = 0; i < animationChannels.size(); i++)
    {
        if (animationChannels[i].myNodeName == animationNode->myName)
        {
            animationNode->myIsAnimated = true;
            animationNode->myChannelIndex = i;
        }
    }
            
    if (!aParentAnimationNode)
        aAnimationData.myRootNode = animationNode;
    else
        aParentAnimationNode->myChildren.push_back(animationNode);

    for (int c = 0; c < aNode->mNumChildren; c++)
        BuildAnimationNodeHierarchy(aNode->mChildren[c], animationNode, aAnimationData);
}
