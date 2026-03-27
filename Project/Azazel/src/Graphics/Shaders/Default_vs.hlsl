struct VS_INPUT
{
    float3 position : POSITION;
    float2 albedoTexCoord : ALBEDOTEXCOORD;
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

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = mul(float4(input.position, 1.0f), WVPMatrix);
    output.albedoTexCoord = input.albedoTexCoord;
    
    return output;
}