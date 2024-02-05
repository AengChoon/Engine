#pragma once
#include <memory>
#include "Bindable.h"
#include "DynamicVertex.h"

class VertexBuffer : public Bindable
{
public:
	VertexBuffer(const Graphics& InGraphics, const DV::VertexBuffer& InVertices);
	VertexBuffer(const Graphics& InGraphics, const std::string& InTag,  const DV::VertexBuffer& InVertices);

	void Bind(const Graphics& InGraphics) noexcept override;

	[[nodiscard]] static std::shared_ptr<VertexBuffer> Resolve(const Graphics& InGraphics, const std::string& InTag, const DV::VertexBuffer& InVertices);

	template<typename... IgnoredParams>
	[[nodiscard]] static std::string GenerateUniqueID(const std::string& InTag, IgnoredParams&&... InIgnoredParams)
	{
		return GenerateUniqueIDImpl(InTag);
	}

	[[nodiscard]] std::string GetUniqueID() const noexcept override;

private:
	[[nodiscard]] static std::string GenerateUniqueIDImpl(const std::string& InTag);

protected:
	std::string Tag;
	UINT Stride;
	Microsoft::WRL::ComPtr<ID3D11Buffer> MyVertexBuffer;
};
