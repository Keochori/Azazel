sampler textureSampler;
Texture2D albedoTexture2D;

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
    if (hasMaterial)
    {
        if (hasAlbedoTexture)
        {
            // Multiply albedoTexture by albedoColor
            float4 albedoTexture = albedoTexture2D.Sample(textureSampler, input.albedoTexCoord);
            return albedoTexture * albedoColor;
        }
        return albedoColor;
    }
    else
    {
        // Missing material
        return float4(1.0, 0.0, 1.0, 1.0);        
    }
}