#include "PixelShader.h"
#include <d3dcompiler.h>
#include "BindManager.h"
#include "ExceptionMacros.h"

PixelShader::PixelShader(const Graphics& InGraphics, const std::string& InFileName)
{
	HRESULT ResultHandle;
	Microsoft::WRL::ComPtr<ID3DBlob> Blob;

	CHECK_HRESULT_EXCEPTION(D3DReadFileToBlob(std::wstring{InFileName.begin(), InFileName.end()}.c_str(), &Blob))
	CHECK_HRESULT_EXCEPTION(GetDevice(InGraphics)->CreatePixelShader
	(
		Blob->GetBufferPointer(),
		Blob->GetBufferSize(),
		nullptr,
		&MyPixelShader
	))
}

void PixelShader::Bind(const Graphics& InGraphics) noexcept
{
	GetContext(InGraphics)->PSSetShader(MyPixelShader.Get(), nullptr, 0u);
}

std::shared_ptr<PixelShader> PixelShader::Resolve(const Graphics& InGraphics, const std::string& InFileName)
{
	return BindManager::Resolve<PixelShader>(InGraphics, InFileName);
}

std::string PixelShader::GenerateUniqueID(const std::string& InFileName)
{
	using namespace std::string_literals;
	return typeid(PixelShader).name() + "#"s + InFileName;
}

std::string PixelShader::GetUniqueID() const noexcept
{
	return GenerateUniqueID(FileName);
}
