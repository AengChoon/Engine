cbuffer LightConstantBuffer
{
    float3 Position;
    float3 AmbientColor;
    float3 DiffuseColor;
    float DiffuseStrength;
    float QuadraticAttenuation;
    float LinearAttenuation;
    float ConstantAttenuation;
}

cbuffer MaterialConstantBuffer
{
    float3 Color;
    float SpecularIntensity;
    float SpecularPower;
}

float4 main(const float3 InWorldPosition : Position, const float3 InNormal : Normal) : SV_TARGET
{
    const float3 VectorToLight = Position - InWorldPosition;
    const float DistanceToLight = length(VectorToLight);
    const float3 DirectionToLight = VectorToLight / DistanceToLight;

    const float Attenuation = 1.0f / (QuadraticAttenuation * (DistanceToLight * DistanceToLight) +
							  LinearAttenuation * DistanceToLight +
							  ConstantAttenuation);

    const float3 Diffuse = DiffuseColor * DiffuseStrength * Attenuation * max(0.0f, dot(DirectionToLight, InNormal));

    const float3 VectorToLightProjectedToNormal = InNormal * dot(VectorToLight, InNormal);
    const float3 VectorToLightReflected = VectorToLightProjectedToNormal * 2.0f - VectorToLight;
    const float3 Specular = Attenuation * (DiffuseColor * DiffuseStrength) * SpecularIntensity * pow(max(0.0f, dot(normalize(-VectorToLightReflected), normalize(InWorldPosition))), SpecularPower);

	return float4(saturate(Diffuse + AmbientColor + Specular) * Color, 1.0f);
}