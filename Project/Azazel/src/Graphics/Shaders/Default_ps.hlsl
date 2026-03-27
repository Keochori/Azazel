sampler textureSampler;
Texture2D albedoTexture;

cbuffer MaterialBuffer
{
    int hasMaterial;
    int hasAlbedoTexture;
    float4 albedoColor;
};

struct PS_INPUT
{
    float4 position : SV_Position;
    float2 albedoTexCoord : ALBEDOTEXCOORD;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    if (hasMaterial == 1)
    {
        if (hasAlbedoTexture == 1)
        {
            // Multiply albedoTexture by albedoColor
            float4 albedo = albedoTexture.Sample(textureSampler, input.albedoTexCoord);
            return albedo * albedoColor;
        }
        return albedoColor;
    }
    else
    {
        // Missing material
        return float4(1.0, 0.0, 1.0, 1.0);        
    }
}