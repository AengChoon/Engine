#pragma once
#include <memory>
#include "Bindable.h"

class IndexBuffer : public Bindable
{
public:
	IndexBuffer(const Graphics& InGraphics, const std::vector<unsigned int>& InIndices);
	IndexBuffer(const Graphics& InGraphics, const std::string& InTag, const std::vector<unsigned int>& InIndices);

	void Bind(const Graphics& InGraphics) noexcept override;
	[[nodiscard]] UINT GetCount() const noexcept;

	[[nodiscard]] static std::shared_ptr<IndexBuffer> Resolve(const Graphics& InGraphics, const std::string& InTag, const std::vector<unsigned int>& InIndices);

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
	UINT Count;
	Microsoft::WRL::ComPtr<ID3D11Buffer> MyIndexBuffer;
};
