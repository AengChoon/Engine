#pragma once
#include "Bindable.h"

class InputLayout : public Bindable
{
public:
	InputLayout(const Graphics& InGraphics, const std::vector<D3D11_INPUT_ELEMENT_DESC>& InLayout,
	            ID3DBlob* InVertexShaderByteCode);

	void Bind(const Graphics& InGraphics) noexcept override
	{
		GetContext(InGraphics)->IASetInputLayout(MyInputLayout.Get());
	}

protected:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> MyInputLayout;
};