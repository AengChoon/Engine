float3 NormalMapToWorldSpace(const float3 InTangent,
							 const float3 InBitangent,
							 const float3 InWorldNormal,
							 const float2 InTextureCoordinate,
							 Texture2D InNormalMap,
							 const SamplerState InSampler)
{
    const float3x3 TangentToWorld = float3x3(InTangent, InBitangent, InWorldNormal);
    const float3 SampledTangentNormal = InNormalMap.Sample(InSampler, InTextureCoordinate).xyz;
    const float3 TangentNormal = SampledTangentNormal * 2.0f - 1.0f;
    return normalize(mul(TangentNormal, TangentToWorld));
}

float CalcAttenuate(const uniform float InQuadraticAttenuation,
					const uniform float InLinearAttenuation,
					const uniform float InConstantAttenuation,
					const in float InDistanceToLight)
{
    return 1.0f / (InQuadraticAttenuation * (InDistanceToLight * InDistanceToLight) + InLinearAttenuation * InDistanceToLight + InConstantAttenuation);
}

float3 CalcDiffuse(const uniform float3 InDiffuseColor,
				   const uniform float InDiffuseStrength,
				   const in float InAttenuation,
				   const in float3 InDirectionToLight,
				   const in float3 InWorldNormal)
{
    return InDiffuseColor * InDiffuseStrength * InAttenuation * max(0.0f, dot(InDirectionToLight, InWorldNormal));
}

float3 CalcSpeculate(const in float3 InSpecularColor,
					 const uniform float InSpecularIntensity,
					 const in float3 InWorldNormal,
					 const in float3 InVectorToLight,
					 const in float3 InVectorToCamera,
					 const in float InAttenuation,
					 const in float InSpecularPower)
{
    const float3 VectorToLightProjectedToNormal = InWorldNormal * dot(InVectorToLight, InWorldNormal);

    // R = 2 * (L  N) - L
    const float3 VectorToLightReflected = normalize(-InVectorToLight + 2.0f * VectorToLightProjectedToNormal);
    const float3 VectorToCamera = normalize(InVectorToCamera);

    return InAttenuation * InSpecularColor * InSpecularIntensity * pow(max(0.0f, dot(VectorToLightReflected, VectorToCamera)), InSpecularPower);
}