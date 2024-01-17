#include "Texture.h"
#include "ExceptionMacros.h"
#include "Surface.h"

Texture::Texture(const Graphics& InGraphics, const Surface& InSurface)
{
	HRESULT ResultHandle;

	D3D11_TEXTURE2D_DESC Texture2DDesc = {};
	Texture2DDesc.Width = InSurface.GetWidth();
	Texture2DDesc.Height = InSurface.GetHeight();
	Texture2DDesc.MipLevels = 1;
	Texture2DDesc.ArraySize = 1;
	Texture2DDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	Texture2DDesc.SampleDesc.Count = 1;
	Texture2DDesc.SampleDesc.Quality = 0;
	Texture2DDesc.Usage = D3D11_USAGE_DEFAULT;
	Texture2DDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	Texture2DDesc.CPUAccessFlags = 0;
	Texture2DDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA SubresourceData = {};
	SubresourceData.pSysMem = InSurface.GetBufferPtrConst();
	SubresourceData.SysMemPitch = InSurface.GetWidth() * sizeof(Surface::Color);

	Microsoft::WRL::ComPtr<ID3D11Texture2D> Texture2D;
	CHECK_HRESULT_EXCEPTION(GetDevice(InGraphics)->CreateTexture2D
	(
		&Texture2DDesc,
		&SubresourceData,
		&Texture2D
	))

	D3D11_SHADER_RESOURCE_VIEW_DESC ResourceViewDesc = {};
	ResourceViewDesc.Format = Texture2DDesc.Format;
	ResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	ResourceViewDesc.Texture2D.MostDetailedMip = 0;
	ResourceViewDesc.Texture2D.MipLevels = 1;
	CHECK_HRESULT_EXCEPTION(GetDevice(InGraphics)->CreateShaderResourceView
	(
		Texture2D.Get(),
		&ResourceViewDesc,
		&MyTextureView
	))
}

void Texture::Bind(const Graphics& InGraphics) noexcept
{
	GetContext(InGraphics)->PSSetShaderResources(0u, 1u, MyTextureView.GetAddressOf());
}