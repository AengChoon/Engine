Texture2D Tex;
SamplerState Sample;

float4 main(const float2 InTextureCoordinate : TextureCoordinate) : SV_Target
{
    return Tex.Sample(Sample, InTextureCoordinate);
}