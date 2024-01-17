#include "PixelShader.h"
#include <d3dcompiler.h>
#include "ExceptionMacros.h"

PixelShader::PixelShader(const Graphics& InGraphics, const std::wstring& InFileName)
{
	HRESULT ResultHandle;
	Microsoft::WRL::ComPtr<ID3DBlob> Blob;

	CHECK_HRESULT_EXCEPTION(D3DReadFileToBlob(InFileName.c_str(), &Blob))
	CHECK_HRESULT_EXCEPTION(GetDevice(InGraphics)->CreatePixelShader
	(
		Blob->GetBufferPointer(),
		Blob->GetBufferSize(),
		nullptr,
		&MyPixelShader
	))
}
