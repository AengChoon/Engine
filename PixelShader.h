#pragma once
#include "Bindable.h"

class PixelShader : public Bindable
{
public:
	PixelShader(const Graphics& InGraphics, const std::wstring& InFileName);

	void Bind(const Graphics& InGraphics) noexcept override
	{
		GetContext(InGraphics)->PSSetShader(MyPixelShader.Get(), nullptr, 0u);
	}

protected:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> MyPixelShader;
};