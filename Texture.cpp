#include "Texture.h"
#include "BindManager.h"
#include "ExceptionMacros.h"
#include "Surface.h"

Texture::Texture(const Graphics& InGraphics, const std::string& InFileName, unsigned int InSlot)
	: FileName(InFileName)
	, Slot(InSlot)
{
	HRESULT ResultHandle;

	const auto Surface = Surface::FromFile(InFileName);

	D3D11_TEXTURE2D_DESC Texture2DDesc = {};
	Texture2DDesc.Width = Surface.GetWidth();
	Texture2DDesc.Height = Surface.GetHeight();
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
	SubresourceData.pSysMem = Surface.GetBufferPtrConst();
	SubresourceData.SysMemPitch = Surface.GetWidth() * sizeof(Surface::Color);

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
	GetContext(InGraphics)->PSSetShaderResources(Slot, 1u, MyTextureView.GetAddressOf());
}

std::shared_ptr<Texture> Texture::Resolve(const Graphics& InGraphics, const std::string& InFileName, unsigned InSlot)
{
	return BindManager::Resolve<Texture>(InGraphics, InFileName, InSlot);
}

std::string Texture::GenerateUniqueID(const std::string& InFileName, unsigned InSlot)
{
	using namespace std::string_literals;
	return typeid(Texture).name() + "#"s + std::to_string(InSlot) + "#"s + InFileName ;
}

std::string Texture::GetUniqueID() const noexcept
{
	return GenerateUniqueID(FileName, Slot);
}
