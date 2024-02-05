#pragma once
#include <memory>
#include "Bindable.h"

class PixelShader : public Bindable
{
public:
	PixelShader(const Graphics& InGraphics, const std::string& InFileName);

	void Bind(const Graphics& InGraphics) noexcept override;

	[[nodiscard]] static std::shared_ptr<PixelShader> Resolve(const Graphics& InGraphics, const std::string& InFileName);
	[[nodiscard]] static std::string GenerateUniqueID(const std::string& InFileName);
	[[nodiscard]] std::string GetUniqueID() const noexcept override;

protected:
	std::string FileName;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> MyPixelShader;
};