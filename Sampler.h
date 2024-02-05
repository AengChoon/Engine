#pragma once
#include <memory>
#include "Bindable.h"

class Sampler : public Bindable
{
public:
	Sampler(const Graphics& InGraphics);

	void Bind(const Graphics& InGraphics) noexcept override;

	[[nodiscard]] static std::shared_ptr<Sampler> Resolve(const Graphics& InGraphics);
	[[nodiscard]] static std::string GenerateUniqueID();
	[[nodiscard]] std::string GetUniqueID() const noexcept override;

protected:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> MySamplerState;
};
