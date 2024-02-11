cbuffer Light
{
    float3 Position;
    float3 AmbientColor;
    float3 DiffuseColor;
    float DiffuseStrength;
    float QuadraticAttenuation;
    float LinearAttenuation;
    float ConstantAttenuation;
}

cbuffer Material
{
    float SpecularIntensity;
    float SpecularPower;
    bool bIsNormalMapEnabled;
    float Padding[1];
}

cbuffer Camera
{
    float3 CameraPosition;
}

Texture2D Texture;
Texture2D NormalMap : register(t2);

SamplerState Sampler;

float4 main(const float3 InWorldPosition : Position,
			float3 InWorldNormal : Normal,
			const float3 InTangent : Tangent,
			const float3 InBitangent : Bitangent,
            const float2 InTextureCoordinate : TexCoord) : SV_TARGET
{
    if (bIsNormalMapEnabled)
    {
        const float3x3 TangentToWorld = float3x3(normalize(InTangent), normalize(InBitangent), normalize(InWorldNormal));
        const float3 NormalSample = NormalMap.Sample(Sampler, InTextureCoordinate).xyz;
        InWorldNormal = NormalSample * 2.0f - 1.0f;
        InWorldNormal.y = -InWorldNormal.y;
        InWorldNormal = normalize(mul(InWorldNormal, TangentToWorld));
    }

    const float3 VectorToLight = Position - InWorldPosition;
    const float DistanceToLight = length(VectorToLight);
    const float3 DirectionToLight = VectorToLight / DistanceToLight;

    const float Attenuation = 1.0f / (QuadraticAttenuation * (DistanceToLight * DistanceToLight) +
							  LinearAttenuation * DistanceToLight +
							  ConstantAttenuation);

    const float3 Diffuse = DiffuseColor * DiffuseStrength * Attenuation * max(0.0f, dot(DirectionToLight, InWorldNormal));

    const float3 VectorToLightProjectedToNormal = InWorldNormal * dot(VectorToLight, InWorldNormal);
    // R = 2 * (L  N) - L
    const float3 VectorToLightReflected = -VectorToLight + 2.0f * VectorToLightProjectedToNormal;
    const float3 Specular = Attenuation * (DiffuseColor * DiffuseStrength) * SpecularIntensity * pow(max(0.0f, dot(normalize(VectorToLightReflected), normalize(CameraPosition - InWorldPosition))), SpecularPower);

    return float4(saturate((Diffuse + AmbientColor) * Texture.Sample(Sampler, InTextureCoordinate).rgb + Specular), 1.0f);
}