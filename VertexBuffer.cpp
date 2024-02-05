#include "VertexBuffer.h"
#include "BindManager.h"
#include "ExceptionMacros.h"

VertexBuffer::VertexBuffer(const Graphics& InGraphics, const DV::VertexBuffer& InVertices)
	: VertexBuffer(InGraphics, "?", InVertices)
{
}

VertexBuffer::VertexBuffer(const Graphics& InGraphics, const std::string& InTag, const DV::VertexBuffer& InVertices)
	: Tag(InTag)
	, Stride(static_cast<UINT>(InVertices.GetLayout().Size()))
{
	HRESULT ResultHandle;

	D3D11_BUFFER_DESC VertexBufferDesc {};
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	VertexBufferDesc.CPUAccessFlags = 0u;
	VertexBufferDesc.MiscFlags = 0u;
	VertexBufferDesc.ByteWidth = static_cast<UINT>(InVertices.Size());
	VertexBufferDesc.StructureByteStride = Stride;

	D3D11_SUBRESOURCE_DATA SubresourceData = {};
	SubresourceData.pSysMem = InVertices.GetData();

	CHECK_HRESULT_EXCEPTION(GetDevice(InGraphics)->CreateBuffer
	(
		&VertexBufferDesc,
		&SubresourceData,
		&MyVertexBuffer
	))
}

void VertexBuffer::Bind(const Graphics& InGraphics) noexcept
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

std::shared_ptr<VertexBuffer> VertexBuffer::Resolve(const Graphics& InGraphics, const std::string& InTag,
                                                    const DV::VertexBuffer& InVertices)
{
	assert(InTag != "?");
	return BindManager::Resolve<VertexBuffer>(InGraphics, InTag, InVertices);
}

std::string VertexBuffer::GenerateUniqueIDImpl(const std::string& InTag)
{
	using namespace std::string_literals;
	return typeid(VertexBuffer).name() + "#"s + InTag;
}

std::string VertexBuffer::GetUniqueID() const noexcept
{
	return GenerateUniqueID(Tag);
}