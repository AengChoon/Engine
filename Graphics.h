#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "wrl/client.h"
#include "DXGIInfoManager.h"

class Camera;

class Graphics
{
	friend class Bindable;

public:
	Graphics(HWND InWindowHandle);
	Graphics(const Graphics&) = delete;
	Graphics(Graphics&&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	Graphics& operator=(Graphics&&) = delete;
	~Graphics();

	void EndFrame();
	void BeginFrame(float InRed = 0.0f, float InGreen = 0.0f, float InBlue = 0.0f) const noexcept;
	void DrawIndexed(UINT InCount) const;

	void EnableImGui() noexcept
	{
		bIsImGuiEnabled = true;
	}

	void DisableImGui() noexcept
	{
		bIsImGuiEnabled = false;
	}

	[[nodiscard]] bool IsImGuiEnabled() const noexcept
	{
		return bIsImGuiEnabled;
	}

	void SetProjectionMatrix(const DirectX::FXMMATRIX& InProjectionMatrix)
	{
		ProjectionMatrix = InProjectionMatrix;
	}

	[[nodiscard]] DirectX::XMMATRIX GetProjectionMatrix() const
	{
		return ProjectionMatrix;
	}

	void SetCamera(const Camera& InCamera)
	{
		Camera = &InCamera;
	}

	[[nodiscard]] const Camera& GetCamera() const
	{
		assert(Camera);
		return *Camera;
	}

private:
	DXGIInfoManager InfoManager;
	DirectX::XMMATRIX ProjectionMatrix;
	DirectX::XMMATRIX CameraMatrix;
	const Camera* Camera;
	Microsoft::WRL::ComPtr<ID3D11Device> Device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> DeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> SwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DepthStencilView;
	bool bIsImGuiEnabled {true};
};
