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
    float3 Color;
    float SpecularIntensity;
    float SpecularPower;
}

cbuffer Camera
{
    float3 CameraPosition;
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
    // R = 2 * (L  N) - L
    const float3 VectorToLightReflected = -VectorToLight + 2.0f * VectorToLightProjectedToNormal;
    const float3 Specular = Attenuation * (DiffuseColor * DiffuseStrength) * SpecularIntensity * pow(max(0.0f, dot(normalize(VectorToLightReflected), normalize(CameraPosition - InWorldPosition))), SpecularPower);

	return float4(saturate(Diffuse + AmbientColor + Specular) * Color, 1.0f);
}