#include "InputLayout.h"
#include "ExceptionMacros.h"

InputLayout::InputLayout(const Graphics& InGraphics, const std::vector<D3D11_INPUT_ELEMENT_DESC>& InLayout,
                         ID3DBlob* InVertexShaderByteCode)
{
    HRESULT ResultHandle;

    CHECK_HRESULT_EXCEPTION(GetDevice(InGraphics)->CreateInputLayout
	(
		InLayout.data(),
		static_cast<UINT>(InLayout.size()),
		InVertexShaderByteCode->GetBufferPointer(),
		InVertexShaderByteCode->GetBufferSize(),
		&MyInputLayout
	))
}
