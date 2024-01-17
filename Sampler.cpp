#include "Sampler.h"
#include "ExceptionMacros.h"

Sampler::Sampler(Graphics& InGraphics)
{
	HRESULT ResultHandle;

	D3D11_SAMPLER_DESC SamplerDesc = {};
	SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	CHECK_HRESULT_EXCEPTION(GetDevice(InGraphics)->CreateSamplerState
	(
		&SamplerDesc,
		&MySamplerState
	))
}

void Sampler::Bind(const Graphics& InGraphics) noexcept
{
	GetContext(InGraphics)->PSSetSamplers(0u, 1u, MySamplerState.GetAddressOf());
}