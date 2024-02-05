#pragma once
#include "Bindable.h"

class VertexShader : public Bindable
{
public:
	VertexShader(const Graphics& InGraphics, const std::string& InFileName);

	void Bind(const Graphics& InGraphics) noexcept override;
	[[nodiscard]] ID3DBlob* GetByteCode() const noexcept;

	[[nodiscard]] static std::shared_ptr<VertexShader> Resolve(const Graphics& InGraphics, const std::string& InFileName);
	[[nodiscard]] static std::string GenerateUniqueID(const std::string& InFileName);
	[[nodiscard]] std::string GetUniqueID() const noexcept override;

protected:
	std::string FileName;
	Microsoft::WRL::ComPtr<ID3DBlob> ByteCodeBlob;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> MyVertexShader;
};