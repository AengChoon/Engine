#include <d3dcompiler.h>
#include <memory>
#include "BindManager.h"
#include "ExceptionMacros.h"
#include "VertexShader.h"

VertexShader::VertexShader(const Graphics& InGraphics, const std::string& InFileName)
	: FileName(InFileName)
{
	HRESULT ResultHandle;

	CHECK_HRESULT_EXCEPTION(D3DReadFileToBlob(std::wstring{InFileName.begin(), InFileName.end()}.c_str(), &ByteCodeBlob))
	CHECK_HRESULT_EXCEPTION(GetDevice(InGraphics)->CreateVertexShader
	(
		ByteCodeBlob->GetBufferPointer(),
		ByteCodeBlob->GetBufferSize(),
		nullptr,
		&MyVertexShader
	))
}

void VertexShader::Bind(const Graphics& InGraphics) noexcept
{
	GetContext(InGraphics)->VSSetShader
	(
		MyVertexShader.Get(),
		nullptr,
		0u
	);
}

ID3DBlob* VertexShader::GetByteCode() const noexcept
{
	return ByteCodeBlob.Get();
}

std::shared_ptr<VertexShader> VertexShader::Resolve(const Graphics& InGraphics, const std::string& InFileName)
{
	return BindManager::Resolve<VertexShader>(InGraphics, InFileName);
}

std::string VertexShader::GenerateUniqueID(const std::string& InFileName)
{
	using namespace std::string_literals;
	return typeid(VertexShader).name() + "#"s + InFileName;
}

std::string VertexShader::GetUniqueID() const noexcept
{
	return GenerateUniqueID(FileName);
}
