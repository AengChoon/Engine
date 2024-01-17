cbuffer CBuf
{
    float4 face_colors[8];
};

float4 main(const uint TriangleID : SV_PrimitiveID) : SV_Target
{
    return face_colors[(TriangleID / 2) % 8];
}