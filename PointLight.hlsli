cbuffer PointLightConstants
{
    float3 LightWorldPosition;
    float3 AmbientColor;
    float3 DiffuseColor;
    float DiffuseStrength;
    float QuadraticAttenuation;
    float LinearAttenuation;
    float ConstantAttenuation;
}