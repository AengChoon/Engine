#include "Topology.h"
#include "BindManager.h"

Topology::Topology(const Graphics& InGraphics, const D3D11_PRIMITIVE_TOPOLOGY InTopologyType)
	: TopologyType(InTopologyType)
{}

void Topology::Bind(const Graphics& InGraphics) noexcept
{
	GetContext(InGraphics)->IASetPrimitiveTopology(TopologyType);
}

std::shared_ptr<Topology> Topology::Resolve(const Graphics& InGraphics, D3D11_PRIMITIVE_TOPOLOGY InTopologyType)
{
	return BindManager::Resolve<Topology>(InGraphics, InTopologyType);
}

std::string Topology::GenerateUniqueID(const D3D11_PRIMITIVE_TOPOLOGY InTopologyType)
{
	using namespace std::string_literals;
	return typeid(Topology).name() + "#"s + std::to_string(InTopologyType);
}

std::string Topology::GetUniqueID() const noexcept
{
	return GenerateUniqueID(TopologyType);
}
