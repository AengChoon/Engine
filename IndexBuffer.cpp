#include "IndexBuffer.h"
#include "ExceptionMacros.h"

IndexBuffer::IndexBuffer(const Graphics& InGraphics, const std::vector<unsigned short>& InIndices)
	: Count(static_cast<UINT>(InIndices.size()))
{
	HRESULT ResultHandle;

	D3D11_BUFFER_DESC IndexBufferDesc {};
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.CPUAccessFlags = 0u;
	IndexBufferDesc.MiscFlags = 0u;
	IndexBufferDesc.ByteWidth = static_cast<UINT>(Count * sizeof(unsigned short));
	IndexBufferDesc.StructureByteStride = sizeof(unsigned short);

	D3D11_SUBRESOURCE_DATA SubresourceData {};
	SubresourceData.pSysMem = InIndices.data();

	CHECK_HRESULT_EXCEPTION(GetDevice(InGraphics)->CreateBuffer
	(
		&IndexBufferDesc,
		&SubresourceData,
		&MyIndexBuffer)
	)
}