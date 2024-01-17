#pragma once
#include "Bindable.h"

class Topology : public Bindable
{
public:
	explicit Topology(const D3D11_PRIMITIVE_TOPOLOGY InType)
		: Type(InType)
	{}

	void Bind(const Graphics& InGraphics) noexcept override
	{
		GetContext(InGraphics)->IASetPrimitiveTopology(Type);
	}

protected:
	D3D11_PRIMITIVE_TOPOLOGY Type;
};