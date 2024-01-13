#pragma once
#include <d3d11.h>
#include "DXGIInfoManager.h"

class Graphics
{
public:
	Graphics(HWND InWindowHandle);
	Graphics(const Graphics&) = delete;
	Graphics(Graphics&&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	Graphics& operator=(Graphics&&) = delete;
	~Graphics();

	void EndFrame();
	void ClearBuffer(float InRed, float InGreen, float InBlue) const noexcept;

private:
	DXGIInfoManager InfoManager;
	ID3D11Device* Device = nullptr;
	ID3D11DeviceContext* DeviceContext = nullptr;
	IDXGISwapChain* SwapChain = nullptr;
	ID3D11RenderTargetView* RenderTargetView = nullptr;
};
