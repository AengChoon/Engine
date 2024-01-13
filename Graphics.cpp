#include "Graphics.h"
#include "EngineException.h"
#pragma comment(lib, "d3d11.lib")

Graphics::Graphics(HWND InWindowHandle)
{
	DXGI_SWAP_CHAIN_DESC SwapChainDesc {};
	SwapChainDesc.BufferDesc.Width = 0;
	SwapChainDesc.BufferDesc.Height = 0;
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
	SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapChainDesc.SampleDesc.Count = 1;
	SwapChainDesc.SampleDesc.Quality = 0;
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.BufferCount = 1;
	SwapChainDesc.OutputWindow = InWindowHandle;
	SwapChainDesc.Windowed = TRUE;
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	SwapChainDesc.Flags = 0;
	
	UINT CreateFlags = 0u;
#ifndef NDEBUG
	CreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	InfoManager.Set();
	const auto CreateResult = D3D11CreateDeviceAndSwapChain
	(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		CreateFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&SwapChainDesc,
		&SwapChain,
		&Device,
		nullptr,
		&DeviceContext
	);
	ResultHandleException::Check(__LINE__, __FILE__, CreateResult, InfoManager.GetMessages()); 

	Microsoft::WRL::ComPtr<ID3D11Resource> BackBuffer;
	InfoManager.Set();
	const auto BackBufferResult = SwapChain->GetBuffer
	(
		0,
		__uuidof(ID3D11Resource),
		&BackBuffer
	);
	ResultHandleException::Check(__LINE__, __FILE__, BackBufferResult, InfoManager.GetMessages());

	InfoManager.Set();
	const auto BackBufferRenderTargetResult = Device->CreateRenderTargetView
	(
		BackBuffer.Get(),
		nullptr,
		&RenderTargetView
	);
	ResultHandleException::Check(__LINE__, __FILE__, BackBufferRenderTargetResult, InfoManager.GetMessages());
}

void Graphics::EndFrame()
{
	InfoManager.Set();
	if (const HRESULT ResultHandle = SwapChain->Present(1u, 0u); FAILED(ResultHandle))
	{
		
		ResultHandle == DXGI_ERROR_DEVICE_REMOVED
							? ResultHandleException::Check(__LINE__, __FILE__, Device->GetDeviceRemovedReason())
							: ResultHandleException::Check(__LINE__, __FILE__, ResultHandle, InfoManager.GetMessages());
	}
}

void Graphics::ClearBuffer(const float InRed, const float InGreen, const float InBlue) const noexcept
{
	const float Color[] = {InRed, InGreen, InBlue, 1.0f};
	DeviceContext->ClearRenderTargetView(RenderTargetView.Get(), Color);
}
