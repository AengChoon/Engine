#include "VertexShader.h"
#include <d3dcompiler.h>
#include "ExceptionMacros.h"

VertexShader::VertexShader(const Graphics& InGraphics, const std::wstring& InFileName)
{
	HRESULT ResultHandle;

	CHECK_HRESULT_EXCEPTION(D3DReadFileToBlob(InFileName.c_str(), &ByteCodeBlob))
	CHECK_HRESULT_EXCEPTION(GetDevice(InGraphics)->CreateVertexShader
	(
		ByteCodeBlob->GetBufferPointer(),
		ByteCodeBlob->GetBufferSize(),
		nullptr,
		&MyVertexShader
	))
}
