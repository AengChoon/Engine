#pragma once
#include <memory>
#include "Bindable.h"
#include "DynamicVertex.h"

class InputLayout : public Bindable
{
public:
	InputLayout(const Graphics& InGraphics, DV::VertexLayout InLayout, ID3DBlob* InVertexShaderByteCode);

	void Bind(const Graphics& InGraphics) noexcept override;

	[[nodiscard]] static std::shared_ptr<InputLayout> Resolve(const Graphics& InGraphics, const DV::VertexLayout& InLayout, ID3DBlob* InVertexShaderByteCode);
	[[nodiscard]] static std::string GenerateUniqueID(const DV::VertexLayout& InLayout, ID3DBlob* InVertexShaderByteCode = nullptr);
	[[nodiscard]] std::string GetUniqueID() const noexcept override;

protected:
	DV::VertexLayout DynamicVertexLayout;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> MyInputLayout;
};
