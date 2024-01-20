cbuffer ModelConstantBuffer
{
    matrix ModelTransform;
    matrix ModelViewProjection;
};

float4 main(float3 InVertexPosition : Position) : SV_Position
{
    return mul(float4(InVertexPosition, 1.0f), ModelViewProjection);
}