#pragma once
#include "Bindable.h"

class IndexBuffer : public Bindable
{
public:
	IndexBuffer(const Graphics& InGraphics, const std::vector<unsigned short>& InIndices);

	void Bind(const Graphics& InGraphics) noexcept override
	{
		GetContext(InGraphics)->IASetIndexBuffer
		(
			MyIndexBuffer.Get(),
			DXGI_FORMAT_R16_UINT,
			0u
		);
	}

	UINT GetCount() const noexcept
	{
		return Count;
	}

protected:
	UINT Count;
	Microsoft::WRL::ComPtr<ID3D11Buffer> MyIndexBuffer;
};
