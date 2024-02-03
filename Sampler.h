#pragma once
#include "Bindable.h"

class Sampler : public Bindable
{
public:
	Sampler(const Graphics& InGraphics);
	void Bind(const Graphics& InGraphics) noexcept override;

protected:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> MySamplerState;
};
