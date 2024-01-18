#include "Graphics.h"
#include <d3dcompiler.h>
#include "ExceptionMacros.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

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

	HRESULT ResultHandle;

	CHECK_HRESULT_EXCEPTION(D3D11CreateDeviceAndSwapChain
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
	))

	Microsoft::WRL::ComPtr<ID3D11Resource> BackBuffer;

	CHECK_HRESULT_EXCEPTION(SwapChain->GetBuffer
	(
		0,
		__uuidof(ID3D11Resource),
		&BackBuffer
	))

	CHECK_HRESULT_EXCEPTION(Device->CreateRenderTargetView
	(
		BackBuffer.Get(),
		nullptr,
		&RenderTargetView
	))

	D3D11_DEPTH_STENCIL_DESC DepthStencilDesc {};
	DepthStencilDesc.DepthEnable = TRUE;
	DepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DepthStencilState;
	CHECK_HRESULT_EXCEPTION(Device->CreateDepthStencilState(&DepthStencilDesc, &DepthStencilState));

	DeviceContext->OMSetDepthStencilState(DepthStencilState.Get(), 1u);

	Microsoft::WRL::ComPtr<ID3D11Texture2D> DepthStencilTexture;
	D3D11_TEXTURE2D_DESC DepthStencilTextureDesc {};
	DepthStencilTextureDesc.Width = 800u;
	DepthStencilTextureDesc.Height = 600u;
	DepthStencilTextureDesc.MipLevels = 1u;
	DepthStencilTextureDesc.ArraySize = 1u;
	DepthStencilTextureDesc.Format = DXGI_FORMAT_D32_FLOAT;
	DepthStencilTextureDesc.SampleDesc.Count = 1u;
	DepthStencilTextureDesc.SampleDesc.Quality = 0u;
	DepthStencilTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	DepthStencilTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	CHECK_HRESULT_EXCEPTION(Device->CreateTexture2D(&DepthStencilTextureDesc, nullptr, &DepthStencilTexture));

	D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc {};
	DepthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DepthStencilViewDesc.Texture2D.MipSlice = 0u;
	CHECK_HRESULT_EXCEPTION(Device->CreateDepthStencilView(DepthStencilTexture.Get(), &DepthStencilViewDesc, &DepthStencilView));

	DeviceContext->OMSetRenderTargets(1u, RenderTargetView.GetAddressOf(), DepthStencilView.Get());

	D3D11_VIEWPORT Viewport;
	Viewport.Width = 800.0f;
	Viewport.Height = 600.0f;
	Viewport.MinDepth = 0.0f;
	Viewport.MaxDepth = 1.0f;
	Viewport.TopLeftX = 0.0f;
	Viewport.TopLeftY = 0.0f;
	DeviceContext->RSSetViewports(1u, &Viewport);

	ImGui_ImplDX11_Init(Device.Get(), DeviceContext.Get());
}

Graphics::~Graphics()
{
	ImGui_ImplDX11_Shutdown();
}

void Graphics::EndFrame()
{
	if (bIsImGuiEnabled)
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	if (const HRESULT ResultHandle = SwapChain->Present(1u, 0u); FAILED(ResultHandle))
	{
		
		if (ResultHandle == DXGI_ERROR_DEVICE_REMOVED)
		{
			throw HRESULT_EXCEPTION(Device->GetDeviceRemovedReason());
		}
		else
		{
			throw HRESULT_EXCEPTION(ResultHandle);
		}
	}
}

void Graphics::BeginFrame(float InRed, float InGreen, float InBlue) const noexcept
{
	if (bIsImGuiEnabled)
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	const float Color[] = {InRed, InGreen, InBlue, 1.0f};
	DeviceContext->ClearRenderTargetView(RenderTargetView.Get(), Color);
	DeviceContext->ClearDepthStencilView(DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void Graphics::DrawIndexed(UINT InCount) const
{
	CHECK_INFO_EXCEPTION(DeviceContext->DrawIndexed(InCount, 0u, 0u))
}
