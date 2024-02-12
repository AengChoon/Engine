#include "PointLight.hlsli"
#include "ShaderOperations.hlsli"

cbuffer Material
{
    float SpecularIntensity;
    float SpecularPower;
    float Padding[2];
}

cbuffer Camera
{
    float3 CameraPosition;
}

Texture2D Texture;
SamplerState Sampler;

float4 main(const float3 InWorldPosition : Position,
			float3 InWorldNormal : Normal,
            const float2 InTextureCoordinate : TexCoord)
			: SV_TARGET
{
    InWorldNormal = normalize(InWorldNormal);

    const float3 VectorToLight = LightWorldPosition - InWorldPosition;
    const float DistanceToLight = length(VectorToLight);
    const float3 DirectionToLight = VectorToLight / DistanceToLight;

    const float Attenuation = CalcAttenuate(QuadraticAttenuation, LinearAttenuation, ConstantAttenuation, DistanceToLight);

    const float3 Diffuse = CalcDiffuse(DiffuseColor, DiffuseStrength, Attenuation, DirectionToLight, InWorldNormal);

    const float3 Specular = CalcSpeculate(DiffuseColor, 1.0f, InWorldNormal, VectorToLight, CameraPosition - InWorldPosition, Attenuation, SpecularPower);

    return float4(saturate((Diffuse + AmbientColor) * Texture.Sample(Sampler, InTextureCoordinate).rgb + Specular), 1.0f);
}