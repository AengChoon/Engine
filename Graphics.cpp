#include "Graphics.h"
#include <d3dcompiler.h>
#include "Exception.h"
#include "ExceptionMacros.h"
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
}

void Graphics::EndFrame()
{
	InfoManager.Set();

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

void Graphics::ClearBuffer(const float InRed, const float InGreen, const float InBlue) const noexcept
{
	const float Color[] = {InRed, InGreen, InBlue, 1.0f};
	DeviceContext->ClearRenderTargetView(RenderTargetView.Get(), Color);
}

void Graphics::DrawTestTriangle()
{
	struct Vertex
	{
		float x;
		float y;
	};

	constexpr Vertex Vertices[] =
	{
		{0.0f, 0.5f},
		{0.5f, -0.5f},
		{-0.5f, -0.5f}
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer;
	D3D11_BUFFER_DESC VertexBufferDescription;
	VertexBufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDescription.Usage = D3D11_USAGE_DEFAULT;
	VertexBufferDescription.CPUAccessFlags = 0u;
	VertexBufferDescription.MiscFlags = 0u;
	VertexBufferDescription.ByteWidth = sizeof(Vertices);
	VertexBufferDescription.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA VertexData;
	VertexData.pSysMem = Vertices;

	InfoManager.Set();
	const auto CreateVertexBufferResult = Device->CreateBuffer(&VertexBufferDescription, &VertexData, &VertexBuffer);
	ResultHandleException::Check(__LINE__, __FILE__, CreateVertexBufferResult, InfoManager.GetMessages());

	constexpr UINT Stride = sizeof(Vertex);
	constexpr UINT Offset = 0u;
	DeviceContext->IASetVertexBuffers(0u, 1u, VertexBuffer.GetAddressOf(), &Stride, &Offset);

	HRESULT ResultHandle;
	Microsoft::WRL::ComPtr<ID3DBlob> Blob;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> PixelShader;

	InfoManager.Set();
	ResultHandle = D3DReadFileToBlob(L"PixelShader.cso", &Blob);
	ResultHandleException::Check(__LINE__, __FILE__, ResultHandle, InfoManager.GetMessages());

	InfoManager.Set();
	ResultHandle = Device->CreatePixelShader(Blob->GetBufferPointer(), Blob->GetBufferSize(), nullptr, &PixelShader);
	ResultHandleException::Check(__LINE__, __FILE__, ResultHandle, InfoManager.GetMessages());
	DeviceContext->PSSetShader(PixelShader.Get(), nullptr, 0u);

	Microsoft::WRL::ComPtr<ID3D11VertexShader> VertexShader;

	DXGIInfoManager::Set();
	ResultHandle = D3DReadFileToBlob(L"VertexShader.cso", &Blob);
	ResultHandleException::Check(__LINE__, __FILE__, ResultHandle, InfoManager.GetMessages());

	InfoManager.Set();
	ResultHandle = Device->CreateVertexShader(Blob->GetBufferPointer(), Blob->GetBufferSize(), nullptr, &VertexShader);
	ResultHandleException::Check(__LINE__, __FILE__, ResultHandle, InfoManager.GetMessages());
	DeviceContext->VSSetShader(VertexShader.Get(), nullptr, 0u);

	Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayout;

	constexpr D3D11_INPUT_ELEMENT_DESC InputElementDescription[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	ResultHandle = Device->CreateInputLayout
	(
		InputElementDescription,
		static_cast<UINT>(std::size(InputElementDescription)),
		Blob->GetBufferPointer(),
		Blob->GetBufferSize(),
		&InputLayout
	);

	DeviceContext->IASetInputLayout(InputLayout.Get());

	DeviceContext->OMSetRenderTargets(1u, RenderTargetView.GetAddressOf(), nullptr);

	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D11_VIEWPORT Viewport;
	Viewport.Width = 800;
	Viewport.Height = 600;
	Viewport.MinDepth = 0;
	Viewport.MaxDepth = 1;
	Viewport.TopLeftX = 0;
	Viewport.TopLeftY = 0;
	DeviceContext->RSSetViewports(1u, &Viewport);

	InfoManager.Set();
	DeviceContext->Draw(static_cast<UINT>(std::size(Vertices)), 0u);
	auto a = InfoManager.GetMessages();
	if (!a.empty())
	{
		throw InfoException{__LINE__, __FILE__, a};
	}
}
