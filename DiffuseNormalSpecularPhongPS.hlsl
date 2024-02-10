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
			float3 InNormal : Normal,
			const float3 InTangent : Tangent,
			const float3 InBitangent : Bitangent,
            const float2 InTextureCoordinate : TexCoord) : SV_TARGET
{
    if (bIsNormalMapEnabled)
    {
        const float3x3 TangentToWorld = float3x3(normalize(InTangent), normalize(InBitangent), normalize(InNormal));
        const float3 NormalSample = NormalMap.Sample(Sampler, InTextureCoordinate).xyz;
        InNormal = NormalSample * 2.0f - 1.0f;
        InNormal = mul(InNormal, TangentToWorld);
    }

    const float3 VectorToLight = Position - InWorldPosition;
    const float DistanceToLight = length(VectorToLight);
    const float3 DirectionToLight = VectorToLight / DistanceToLight;

    const float Attenuation = 1.0f / (QuadraticAttenuation * (DistanceToLight * DistanceToLight) +
							  LinearAttenuation * DistanceToLight +
							  ConstantAttenuation);

    const float3 Diffuse = DiffuseColor * DiffuseStrength * Attenuation * max(0.0f, dot(DirectionToLight, InNormal));

    const float3 VectorToLightProjectedToNormal = InNormal * dot(VectorToLight, InNormal);
    // R = 2 * (L  N) - L
    const float3 VectorToLightReflected = -VectorToLight + 2.0f * VectorToLightProjectedToNormal;

    const float4 SpecularSample = SpecularMap.Sample(Sampler, InTextureCoordinate);
    const float3 SpecularReflectionColor = SpecularSample.rgb;
    const float SpecularPower = pow(2.0f, SpecularSample.a * 13.0f);
    const float3 Specular = Attenuation * (DiffuseColor * DiffuseStrength) * pow(max(0.0f, dot(normalize(VectorToLightReflected), normalize(CameraPosition - InWorldPosition))), SpecularPower);

    return float4(saturate((Diffuse + AmbientColor) * DiffuseMap.Sample(Sampler, InTextureCoordinate).rgb + Specular * SpecularReflectionColor), 1.0f);
}