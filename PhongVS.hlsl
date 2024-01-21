cbuffer Transform
{
    matrix ModelView;
    matrix ModelViewProjection;
};

struct VSOutput
{
    float3 VertexCameraPosition : Position;
    float3 NormalCameraPosition : Normal;
    float4 VertexPosition : SV_Position;
};

VSOutput main(const float3 InModelPosition : Position, const float3 InNormal : Normal)
{
    VSOutput VSOutput;
    VSOutput.VertexCameraPosition = (float3) mul(float4(InModelPosition, 1.0f), ModelView);
    VSOutput.NormalCameraPosition = mul(InNormal, (float3x3) ModelView);
    VSOutput.VertexPosition = mul(float4(InModelPosition, 1.0f), ModelViewProjection);

	return VSOutput;
}