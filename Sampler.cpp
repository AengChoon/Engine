#include "Sampler.h"

#include "BindManager.h"
#include "ExceptionMacros.h"

Sampler::Sampler(const Graphics& InGraphics)
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

std::shared_ptr<Sampler> Sampler::Resolve(const Graphics& InGraphics)
{
	return BindManager::Resolve<Sampler>(InGraphics);
}

std::string Sampler::GenerateUniqueID()
{
	return typeid(Sampler).name();
}

std::string Sampler::GetUniqueID() const noexcept
{
	return GenerateUniqueID();
}
