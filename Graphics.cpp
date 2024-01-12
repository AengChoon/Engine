#include "Graphics.h"
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

	D3D11CreateDeviceAndSwapChain
	(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&SwapChainDesc,
		&SwapChain,
		&Device,
		nullptr,
		&DeviceContext
	);

	ID3D11Resource* BackBuffer = nullptr;
	SwapChain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&BackBuffer));
	Device->CreateRenderTargetView(BackBuffer, nullptr, &RenderTargetView);
	BackBuffer->Release();
}

Graphics::~Graphics()
{
	if (Device) { Device->Release(); }
	if (DeviceContext) { DeviceContext->Release(); }
	if (SwapChain) { SwapChain->Release(); }
	if (RenderTargetView) { RenderTargetView->Release(); }
}

void Graphics::EndFrame()
{
	SwapChain->Present(1u, 0u);
}

void Graphics::ClearBuffer(float InRed, float InGreen, float InBlue) noexcept
{
	const float Color[] = {InRed, InGreen, InBlue, 1.0f};
	DeviceContext->ClearRenderTargetView(RenderTargetView, Color);
}
