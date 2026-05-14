#define MAX_BONES_PER_VERTEX 4 // When changing this number, apply changes to Vertex struct accordingly
#define MAX_BONES 128 // When changing this number, apply changes to constant buffer structs accordingly

struct VS_INPUT
{
    float3 position : POSITION;
    float2 albedoTexCoord : ALBEDOTEXCOORD;
    uint4 boneIDs : BONEIDS;
    float4 boneWeights : BONEWEIGHTS;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float2 albedoTexCoord : ALBEDOTEXCOORD;
};

cbuffer WVPBuffer
{
    matrix WVPMatrix;
};

cbuffer BoneBuffer
{
    int hasSkeleton;
    matrix finalBoneMatrices[MAX_BONES];
};

VS_OUTPUT main(VS_INPUT input)
{
    float4 position = float4(input.position, 1.0f);

    // Animate vertex
    if (hasSkeleton)
    {
        matrix boneInfluenceMatrix = (matrix)0;
        for (int i = 0; i < MAX_BONES_PER_VERTEX; i++)
        {
            float boneWeight = input.boneWeights[i];
            if (boneWeight > 0.0f)
                boneInfluenceMatrix += finalBoneMatrices[input.boneIDs[i]] * boneWeight;
        }
        
        position = mul(position, boneInfluenceMatrix);
    }
    
    VS_OUTPUT output;
    output.position = mul(position, WVPMatrix);
    output.albedoTexCoord = input.albedoTexCoord;
    
    return output;
}