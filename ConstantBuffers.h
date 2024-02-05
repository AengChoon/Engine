#pragma once
#include "Bindable.h"
#include "BindManager.h"
#include "ExceptionMacros.h"

template<typename T>
class ConstantBuffer : public Bindable
{
public:
	ConstantBuffer(const Graphics& InGraphics, const T& InConstants, const UINT InSlot = 0u)
		: Slot(InSlot)
	{
		HRESULT ResultHandle;

		D3D11_BUFFER_DESC ConstantBufferDescription;
		ConstantBufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		ConstantBufferDescription.Usage = D3D11_USAGE_DYNAMIC;
		ConstantBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		ConstantBufferDescription.MiscFlags = 0u;
		ConstantBufferDescription.ByteWidth = sizeof(InConstants);
		ConstantBufferDescription.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA SubresourceData = {};
		SubresourceData.pSysMem = &InConstants;

		CHECK_HRESULT_EXCEPTION(GetDevice(InGraphics)->CreateBuffer
			(
				&ConstantBufferDescription,
				&SubresourceData,
				&MyConstantBuffer
			))
	}

	explicit ConstantBuffer(const Graphics& InGraphics, const UINT InSlot = 0u)
		: Slot(InSlot)
	{
		HRESULT ResultHandle;

		D3D11_BUFFER_DESC ConstantBufferDescription;
		ConstantBufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		ConstantBufferDescription.Usage = D3D11_USAGE_DYNAMIC;
		ConstantBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		ConstantBufferDescription.MiscFlags = 0u;
		ConstantBufferDescription.ByteWidth = sizeof(T);
		ConstantBufferDescription.StructureByteStride = 0u;

		CHECK_HRESULT_EXCEPTION(GetDevice(InGraphics)->CreateBuffer
			(
				&ConstantBufferDescription,
				nullptr,
				&MyConstantBuffer
			))
	}

	void Update(const Graphics& InGraphics, const T& InConstants)
	{
		HRESULT ResultHandle;

		D3D11_MAPPED_SUBRESOURCE MappedSubresource;
		CHECK_HRESULT_EXCEPTION(GetContext(InGraphics)->Map
		(
			MyConstantBuffer.Get(), 
			0u,
			D3D11_MAP_WRITE_DISCARD,
			0u,
			&MappedSubresource
		))

		memcpy(MappedSubresource.pData, &InConstants, sizeof(InConstants));
		GetContext(InGraphics)->Unmap(MyConstantBuffer.Get(), 0u);
	}

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> MyConstantBuffer;
	UINT Slot;
};

template<typename T>
class VertexConstantBuffer final : public ConstantBuffer<T>
{
public:
	using ConstantBuffer<T>::ConstantBuffer;

	void Bind(const Graphics& InGraphics) noexcept override
	{
		ConstantBuffer<T>::GetContext(InGraphics)->VSSetConstantBuffers
		(
			ConstantBuffer<T>::Slot,
			1u,
			ConstantBuffer<T>::MyConstantBuffer.GetAddressOf()
		);
	}

	[[nodiscard]] static std::shared_ptr<VertexConstantBuffer> Resolve(const Graphics& InGraphics, const T& InConstants, UINT InSlot = 0)
	{
		return BindManager::Resolve<VertexConstantBuffer>(InGraphics, InConstants, InSlot);
	}

	[[nodiscard]] static std::shared_ptr<VertexConstantBuffer> Resolve(const Graphics& InGraphics, UINT InSlot = 0)
	{
		return BindManager::Resolve<VertexConstantBuffer>(InGraphics, InSlot);
	}

	[[nodiscard]] static std::string GenerateUniqueID(const T& InConstants, const UINT InSlot)
	{
		return GenerateUniqueID(InSlot);
	}

	[[nodiscard]] static std::string GenerateUniqueID(const UINT InSlot = 0)
	{
		using namespace std::string_literals;
		return typeid(VertexConstantBuffer).name() + "#"s + std::to_string(InSlot);
	}

	[[nodiscard]] std::string GetUniqueID() const noexcept override
	{
		return GenerateUniqueID(ConstantBuffer<T>::Slot);
	}
};

template<typename T>
class PixelConstantBuffer final : public ConstantBuffer<T>
{
public:
	using ConstantBuffer<T>::ConstantBuffer;

	void Bind(const Graphics& InGraphics) noexcept override
	{
		ConstantBuffer<T>::GetContext(InGraphics)->PSSetConstantBuffers
		(
			ConstantBuffer<T>::Slot,
			1u,
			ConstantBuffer<T>::MyConstantBuffer.GetAddressOf()
		);
	}

	[[nodiscard]] static std::shared_ptr<PixelConstantBuffer> Resolve(const Graphics& InGraphics, const T& InConstants, UINT InSlot = 0)
	{
		return BindManager::Resolve<PixelConstantBuffer>(InGraphics, InConstants, InSlot);
	}

	[[nodiscard]] static std::shared_ptr<PixelConstantBuffer> Resolve(const Graphics& InGraphics, UINT InSlot = 0)
	{
		return BindManager::Resolve<PixelConstantBuffer>(InGraphics, InSlot);
	}

	[[nodiscard]] static std::string GenerateUniqueID(const T& InConstants, const UINT InSlot)
	{
		return GenerateUniqueID(InSlot);
	}

	[[nodiscard]] static std::string GenerateUniqueID(const UINT InSlot = 0)
	{
		using namespace std::string_literals;
		return typeid(PixelConstantBuffer).name() + "#"s + std::to_string(InSlot);
	}

	[[nodiscard]] std::string GetUniqueID() const noexcept override
	{
		return GenerateUniqueID(ConstantBuffer<T>::Slot);
	}
};