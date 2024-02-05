#include "InputLayout.h"
#include "BindManager.h"
#include "ExceptionMacros.h"

InputLayout::InputLayout(const Graphics& InGraphics, DV::VertexLayout InLayout, ID3DBlob* InVertexShaderByteCode)
	: DynamicVertexLayout(std::move(InLayout))
{
    HRESULT ResultHandle;

	const auto& InputElementDescs = DynamicVertexLayout.GetD3D11Layout();

    CHECK_HRESULT_EXCEPTION(GetDevice(InGraphics)->CreateInputLayout
	(
		InputElementDescs.data(),
		static_cast<UINT>(InputElementDescs.size()),
		InVertexShaderByteCode->GetBufferPointer(),
		InVertexShaderByteCode->GetBufferSize(),
		&MyInputLayout
	))
}

void InputLayout::Bind(const Graphics& InGraphics) noexcept
{
	GetContext(InGraphics)->IASetInputLayout(MyInputLayout.Get());
}

std::shared_ptr<InputLayout> InputLayout::Resolve(const Graphics& InGraphics, const DV::VertexLayout& InLayout,
                                               ID3DBlob* InVertexShaderByteCode)
{
	return BindManager::Resolve<InputLayout>(InGraphics, InLayout, InVertexShaderByteCode);
}

std::string InputLayout::GenerateUniqueID(const DV::VertexLayout& InLayout, ID3DBlob* InVertexShaderByteCode)
{
	using namespace std::string_literals;
	return typeid(InputLayout).name() + "#"s + InLayout.GetCode();
}

std::string InputLayout::GetUniqueID() const noexcept
{
	return GenerateUniqueID(DynamicVertexLayout);
}