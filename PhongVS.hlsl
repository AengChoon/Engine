cbuffer Transform
{
    matrix Model;
    matrix ModelViewProjection;
};

struct VSOutput
{
    float3 VertexWorldPosition : Position;
    float3 NormalWorldPosition : Normal;
    float2 TextureCoordinate : TexCoord;
    float4 VertexPosition : SV_Position;
};

VSOutput main(const float3 InModelPosition : Position, const float3 InNormal : Normal,
              float2 InTextureCoordinate : TexCoord)
{
    VSOutput VSOutput;
    VSOutput.VertexWorldPosition = (float3) mul(float4(InModelPosition, 1.0f), Model);
    VSOutput.NormalWorldPosition = normalize(mul(InNormal, (float3x3) Model));
    VSOutput.VertexPosition = mul(float4(InModelPosition, 1.0f), ModelViewProjection);
    VSOutput.TextureCoordinate = InTextureCoordinate;

	return VSOutput;
}