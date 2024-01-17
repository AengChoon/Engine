#pragma once
#include "Bindable.h"

class VertexShader : public Bindable
{
public:
	VertexShader(const Graphics& InGraphics, const std::wstring& InFileName);

	void Bind(const Graphics& InGraphics) noexcept override
	{
		GetContext(InGraphics)->VSSetShader
		(
			MyVertexShader.Get(),
			nullptr,
			0u
		);
	}

	[[nodiscard]] ID3DBlob* GetByteCode() const noexcept
	{
		return ByteCodeBlob.Get();
	}

protected:
	Microsoft::WRL::ComPtr<ID3DBlob> ByteCodeBlob;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> MyVertexShader;
};