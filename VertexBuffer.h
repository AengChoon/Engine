#pragma once
#include "Bindable.h"
#include "ExceptionMacros.h"

class VertexBuffer : public Bindable
{
public:
	template<class T>
	VertexBuffer(const Graphics& InGraphics, const std::vector<T>& InVertices)
		: Stride(sizeof(T))
	{
		HRESULT ResultHandle;

		D3D11_BUFFER_DESC VertexBufferDesc {};
		VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		VertexBufferDesc.CPUAccessFlags = 0u;
		VertexBufferDesc.MiscFlags = 0u;
		VertexBufferDesc.ByteWidth = static_cast<UINT>(sizeof(T) * InVertices.size());
		VertexBufferDesc.StructureByteStride = sizeof(T);

		D3D11_SUBRESOURCE_DATA SubresourceData = {};
		SubresourceData.pSysMem = InVertices.data();

		CHECK_HRESULT_EXCEPTION(GetDevice(InGraphics)->CreateBuffer
		(
			&VertexBufferDesc,
			&SubresourceData,
			&MyVertexBuffer
		))
	}

	void Bind(const Graphics& InGraphics) noexcept override
	{
		constexpr UINT Offset = 0u;

		GetContext(InGraphics)->IASetVertexBuffers
		(
			0u,
			1u,
			MyVertexBuffer.GetAddressOf(),
			&Stride,
			&Offset
		);
	}

protected:
	UINT Stride;
	Microsoft::WRL::ComPtr<ID3D11Buffer> MyVertexBuffer;
};
