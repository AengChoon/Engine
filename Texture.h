#pragma once
#include <memory>
#include "Bindable.h"

class Texture : public Bindable
{
public:
	Texture(const Graphics& InGraphics, const std::string& InFileName, unsigned int InSlot = 0);

	void Bind(const Graphics& InGraphics) noexcept override;

	[[nodiscard]] static std::shared_ptr<Texture> Resolve(const Graphics& InGraphics, const std::string& InFileName, unsigned int InSlot = 0);
	[[nodiscard]] static std::string GenerateUniqueID(const std::string& InFileName, unsigned int InSlot = 0);
	[[nodiscard]] std::string GetUniqueID() const noexcept override;

protected:
	std::string FileName;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> MyTextureView;

private:
	unsigned int Slot;
};
