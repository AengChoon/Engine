#include "PointLight.hlsli"
#include "ShaderOperations.hlsli"

cbuffer Material
{
    bool bIsNormalMapEnabled;
    float Padding[3];
}

cbuffer Camera
{
    float3 CameraPosition;
}

Texture2D DiffuseMap;
Texture2D NormalMap;
Texture2D SpecularMap;

SamplerState Sampler;

float4 main(const float3 InWorldPosition : Position,
			float3 InWorldNormal : Normal,
			const float3 InWorldTangent : Tangent,
			const float3 InWorldBitangent : Bitangent,
            const float2 InTextureCoordinate : TexCoord) : SV_TARGET
{
    InWorldNormal = normalize(InWorldNormal);

    if (bIsNormalMapEnabled)
    {
        InWorldNormal = NormalMapToWorldSpace
    				    (
					normalize(InWorldTangent),
							 normalize(InWorldBitangent),
							 InWorldNormal,
							 InTextureCoordinate,
							 NormalMap,
							 Sampler
						);
    }

    const float3 VectorToLight = LightWorldPosition - InWorldPosition;
    const float DistanceToLight = length(VectorToLight);
    const float3 DirectionToLight = VectorToLight / DistanceToLight;

    const float Attenuation = CalcAttenuate(QuadraticAttenuation, LinearAttenuation, ConstantAttenuation, DistanceToLight);

    const float3 Diffuse = CalcDiffuse(DiffuseColor, DiffuseStrength, Attenuation, DirectionToLight, InWorldNormal);

    const float4 SpecularSample = SpecularMap.Sample(Sampler, InTextureCoordinate);
    const float3 SpecularReflectionColor = SpecularSample.rgb;
    const float SpecularPower = pow(2.0f, SpecularSample.a * 13.0f);
    const float3 Specular = CalcSpeculate(SpecularReflectionColor, 1.0f, InWorldNormal, VectorToLight, CameraPosition - InWorldPosition, Attenuation, SpecularPower);

    return float4(saturate((Diffuse + AmbientColor) * DiffuseMap.Sample(Sampler, InTextureCoordinate).rgb + Specular * SpecularReflectionColor), 1.0f);
}