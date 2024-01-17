cbuffer CBuf
{
    matrix Transform;
};

struct VSOut
{
    float2 TextureCoordinate : TextureCoordinate;
    float4 VertexPosition : SV_Position;
};

VSOut main(float3 InVertexPosition : Position, const float2 InTextureCoordinate : TextureCoordinate)
{
    VSOut Out;
    Out.VertexPosition = mul(float4(InVertexPosition, 1.0f), Transform);
    Out.TextureCoordinate = InTextureCoordinate;
    return Out;
}