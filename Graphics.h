#pragma once
#include <d3d11.h>
#include "wrl/client.h"
#include "DXGIInfoManager.h"

class Graphics
{
public:
	Graphics(HWND InWindowHandle);
	Graphics(const Graphics&) = delete;
	Graphics(Graphics&&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	Graphics& operator=(Graphics&&) = delete;
	~Graphics() = default;

	void EndFrame();
	void ClearBuffer(float InRed, float InGreen, float InBlue) const noexcept;

private:
	DXGIInfoManager InfoManager;
	
	Microsoft::WRL::ComPtr<ID3D11Device> Device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> DeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> SwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RenderTargetView;
};
