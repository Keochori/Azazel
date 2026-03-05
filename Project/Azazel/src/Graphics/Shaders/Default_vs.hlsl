struct VSOut
{
    float3 color : COLOR;
    float4 pos : SV_POSITION;
};

cbuffer CBuf
{
    row_major matrix transform;
};

VSOut main(float2 pos : POSITION, float3 color : COLOR)
{
    VSOut vso;
    vso.color = float3(color.r, color.g, color.b);
    vso.pos = mul(float4(pos.x, pos.y, 0.0f, 1.0f), transform);
    return vso;
}