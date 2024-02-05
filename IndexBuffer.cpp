#include "IndexBuffer.h"
#include "BindManager.h"
#include "ExceptionMacros.h"

IndexBuffer::IndexBuffer(const Graphics& InGraphics, const std::vector<unsigned int>& InIndices)
	: IndexBuffer(InGraphics, "?", InIndices)
{
}

IndexBuffer::IndexBuffer(const Graphics& InGraphics, const std::string& InTag, const std::vector<unsigned>& InIndices)
	: Tag(InTag)
	, Count(static_cast<UINT>(InIndices.size()))
{
	HRESULT ResultHandle;

	D3D11_BUFFER_DESC IndexBufferDesc {};
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.CPUAccessFlags = 0u;
	IndexBufferDesc.MiscFlags = 0u;
	IndexBufferDesc.ByteWidth = static_cast<UINT>(Count * sizeof(unsigned int));
	IndexBufferDesc.StructureByteStride = sizeof(unsigned int);

	D3D11_SUBRESOURCE_DATA SubresourceData {};
	SubresourceData.pSysMem = InIndices.data();

	CHECK_HRESULT_EXCEPTION(GetDevice(InGraphics)->CreateBuffer
	(
		&IndexBufferDesc,
		&SubresourceData,
		&MyIndexBuffer)
	)
}

void IndexBuffer::Bind(const Graphics& InGraphics) noexcept
{
	GetContext(InGraphics)->IASetIndexBuffer
	(
		MyIndexBuffer.Get(),
		DXGI_FORMAT_R32_UINT,
		0u
	);
}

UINT IndexBuffer::GetCount() const noexcept
{
	return Count;
}

std::shared_ptr<IndexBuffer> IndexBuffer::Resolve(const Graphics& InGraphics, const std::string& InTag,
                                                  const std::vector<unsigned>& InIndices)
{
	assert(InTag != "?");
	return BindManager::Resolve<IndexBuffer>(InGraphics, InTag, InIndices);
}

std::string IndexBuffer::GenerateUniqueIDImpl(const std::string& InTag)
{
	using namespace std::string_literals;
	return typeid(IndexBuffer).name() + "#"s + InTag;
}

std::string IndexBuffer::GetUniqueID() const noexcept
{
	return GenerateUniqueID(Tag);
}
