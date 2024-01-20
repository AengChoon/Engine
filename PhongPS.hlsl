cbuffer LightConstantBuffer
{
    float3 LightPosition;
}

static const float3 Ambient = { 0.05f, 0.05f, 0.15f };
static const float3 DiffuseColor = { 1.0f, 1.0f, 1.0f };
static const float DiffuseIntensity = 1.0f;
static const float ConstantAttenuation = 1.0f;
static const float LinearAttenuation = 0.045f;
static const float QuadraticAttenuation = 0.0075f;

float4 main(const float3 InWorldPosition : Position, const float3 InNormal : Normal) : SV_TARGET
{
    const float3 VectorToLight = LightPosition - InWorldPosition;
    const float DistanceToLight = length(VectorToLight);
    const float3 DirectionToLight = VectorToLight / DistanceToLight;

    const float Attenuation = 1.0f / (QuadraticAttenuation * (DistanceToLight * DistanceToLight) +
							  LinearAttenuation * DistanceToLight +
							  ConstantAttenuation);

    const float3 Diffuse = DiffuseColor * DiffuseIntensity * Attenuation * max(0.0f, dot(DirectionToLight, InNormal));

	return float4(saturate(Diffuse + Ambient), 1.0f);
}