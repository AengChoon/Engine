#pragma once
#include <memory>
#include "Bindable.h"

class Topology : public Bindable
{
public:
	explicit Topology(const Graphics& InGraphics, D3D11_PRIMITIVE_TOPOLOGY InTopologyType);

	void Bind(const Graphics& InGraphics) noexcept override;

	[[nodiscard]] static std::shared_ptr<Topology> Resolve(const Graphics& InGraphics, D3D11_PRIMITIVE_TOPOLOGY InTopologyType);
	[[nodiscard]] static std::string GenerateUniqueID(D3D11_PRIMITIVE_TOPOLOGY InTopologyType);
	[[nodiscard]] std::string GetUniqueID() const noexcept override;

protected:
	D3D11_PRIMITIVE_TOPOLOGY TopologyType;
};