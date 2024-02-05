#pragma once
#include "Graphics.h"

class Bindable
{
public:
	Bindable() = default;
	Bindable(const Bindable&) = delete;
    Bindable(const Bindable&&) = delete;
    Bindable& operator=(const Bindable&) = delete;
    Bindable& operator=(const Bindable&&) = delete;
	virtual ~Bindable() = default;

	virtual void Bind(const Graphics& InGraphics) noexcept = 0;
	virtual std::string GetUniqueID() const noexcept
	{
		assert(false);
		return {};
	}

protected:
	static ID3D11DeviceContext* GetContext(const Graphics& InGraphics) noexcept
	{
		return InGraphics.DeviceContext.Get();
	}

	static ID3D11Device* GetDevice(const Graphics& InGraphics) noexcept
	{
		return InGraphics.Device.Get();
	}
};
