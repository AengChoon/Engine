#pragma once
#include <DirectXMath.h>
#include <vector>

struct BGRAColor
{
	unsigned char A;
	unsigned char R;
	unsigned char G;
	unsigned char B;
};

class VertexLayout
{
public:
	enum class ElementType
	{
		Position2D,
		Position3D,
		Texture2D,
		Normal,
		Float3Color,
		Float4Color,
		BGRAColor
	};

	class Element
	{
	private:
		ElementType Type;
		size_t Offset;

	public:
		Element(const ElementType InType, const size_t InOffset)
			: Type(InType), Offset(InOffset)
		{}

		[[nodiscard]] ElementType GetType() const
		{
			return Type;
		}

		[[nodiscard]] size_t GetOffset() const
		{
			return Offset;
		}

		[[nodiscard]] size_t GetOffsetAfter() const
		{
			return Offset + Size();
		}

		[[nodiscard]] size_t Size() const
		{
			return SizeOf(Type);
		}

		static constexpr size_t SizeOf(const ElementType InType)
		{
			switch (InType)
			{
			case ElementType::Position2D:
				return sizeof(DirectX::XMFLOAT2);
			case ElementType::Position3D:
				return sizeof(DirectX::XMFLOAT3);
			case ElementType::Texture2D:
				return sizeof(DirectX::XMFLOAT2);
			case ElementType::Normal:
				return sizeof(DirectX::XMFLOAT3);
			case ElementType::Float3Color:
				return sizeof(DirectX::XMFLOAT3);
			case ElementType::Float4Color:
				return sizeof(DirectX::XMFLOAT4);
			case ElementType::BGRAColor:
				return sizeof(BGRAColor);
			default:
				assert(false && "Invalid Element Type");
				return NULL;
			}
		}
	};

private:
	std::vector<Element> Elements;

public:
	template<ElementType T>
	VertexLayout& Append()
	{
		Elements.emplace_back(T, Size());
		return *this;
	}

	template<ElementType T>
	[[nodiscard]] const Element& Resolve() const
	{
		if (auto Result = std::ranges::find_if(Elements, [](const Element& InElement)
		{
			return InElement.GetType() == T;
		}); Result != Elements.end())
		{
			return *Result;
		}
		else
		{
			assert(false && "Could Not Resolve Element Type");
			return Elements.front();
		}
	}

	[[nodiscard]] const Element& ResolveByIndex(const size_t InIndex) const
	{
		return Elements[InIndex];
	}

	[[nodiscard]] size_t Size() const
	{
		return Elements.empty() ? NULL : Elements.back().GetOffsetAfter();
	}

	[[nodiscard]] size_t GetElementCount() const
	{
		return Elements.size();
	}
};

class Vertex
{
	friend class VertexBuffer;

private:
	char* Data {nullptr};
	std::reference_wrapper<const VertexLayout> Layout;

public:
	template<VertexLayout::ElementType ElementType>
	auto& Attribute()
	{
		const auto& Element = Layout.get().Resolve<ElementType>();
		auto Attribute = Data + Element.GetOffset();

		if constexpr (ElementType == VertexLayout::ElementType::Position2D)
		{
			return *reinterpret_cast<DirectX::XMFLOAT2*>(Attribute);
		}
		else if constexpr (ElementType == VertexLayout::ElementType::Position3D)
		{
			return *reinterpret_cast<DirectX::XMFLOAT3*>(Attribute);
		}
		else if constexpr (ElementType == VertexLayout::ElementType::Texture2D)
		{
			return *reinterpret_cast<DirectX::XMFLOAT2*>(Attribute);
		}
		else if constexpr (ElementType == VertexLayout::ElementType::Normal)
		{
			return *reinterpret_cast<DirectX::XMFLOAT3*>(Attribute);
		}
		else if constexpr (ElementType == VertexLayout::ElementType::Float3Color)
		{
			return *reinterpret_cast<DirectX::XMFLOAT3*>(Attribute);
		}
		else if constexpr (ElementType == VertexLayout::ElementType::Float4Color)
		{
			return *reinterpret_cast<DirectX::XMFLOAT4*>(Attribute);
		}
		else if constexpr (ElementType == VertexLayout::ElementType::BGRAColor)
		{
			return *reinterpret_cast<BGRAColor*>(Attribute);
		}
		else
		{
			assert(false && "Invalid Element Type");
			return *reinterpret_cast<char*>(Attribute);
		}
	}

	template<typename T>
	void SetAttributeByIndex(const size_t InIndex, T&& InValue)
	{
		const auto& Element = Layout.get().ResolveByIndex(InIndex);
		auto Attribute = Data + Element.GetOffset();

		switch (Element.GetType())
		{
		case VertexLayout::ElementType::Position2D:
			SetAttribute<DirectX::XMFLOAT2>(Attribute, std::forward<T>(InValue));
			break;
		case VertexLayout::ElementType::Position3D:
			SetAttribute<DirectX::XMFLOAT3>(Attribute, std::forward<T>(InValue));
			break;
		case VertexLayout::ElementType::Texture2D:
			SetAttribute<DirectX::XMFLOAT2>(Attribute, std::forward<T>(InValue));
			break;
		case VertexLayout::ElementType::Normal:
			SetAttribute<DirectX::XMFLOAT3>(Attribute, std::forward<T>(InValue));
			break;
		case VertexLayout::ElementType::Float3Color:
			SetAttribute<DirectX::XMFLOAT3>(Attribute, std::forward<T>(InValue));
			break;
		case VertexLayout::ElementType::Float4Color:
			SetAttribute<DirectX::XMFLOAT4>(Attribute, std::forward<T>(InValue));
			break;
		case VertexLayout::ElementType::BGRAColor:
			SetAttribute<BGRAColor>(Attribute, std::forward<T>(InValue));
			break;
		default: assert(false && "Invalid Element Type");
		}
	}

protected:
	Vertex(char* InData, const VertexLayout& InLayout)
		: Data(InData), Layout(InLayout)
	{
		assert(InData);
	}

private:
	template<typename Destination, typename Source>
	void SetAttribute(char* InAttribute, Source&& InValue) 
	{
		if constexpr (std::is_assignable_v<Destination, Source>)
		{
			*reinterpret_cast<Destination*>(InAttribute) = InValue;
		}
		else
		{
			assert(false && "Attribute type and value type mismatch");
		}
	}

	template<typename First, typename ...Rest>
	void SetAttributeByIndex(size_t Index, First&& InFirst, Rest&&... InRest)
	{
		SetAttributeByIndex(Index, std::forward<First>(InFirst));
		SetAttributeByIndex(Index + 1, std::forward<Rest>(InRest)...);
	}
};

class VertexBuffer
{
private:
	std::vector<char> Buffer;
	VertexLayout Layout;

public:
	explicit VertexBuffer(VertexLayout InLayout)
		: Layout(std::move(InLayout))
	{}

	[[nodiscard]] const VertexLayout& GetLayout() const
	{
		return Layout;
	}

	[[nodiscard]] size_t Size() const
	{
		return Buffer.size() / Layout.Size();
	}

	template<typename ...Params>
	void Emplace(Params&&... InParams)
	{
		assert(sizeof...(InParams) == Layout.GetElementCount() && "Parameter count doesn't match vertex count");
		Buffer.resize(Buffer.size() + Layout.Size());
		Back().SetAttributeByIndex(0u, std::forward<Params>(InParams)...);
	}

	Vertex Back()
	{
		assert(!Buffer.empty());
		return Vertex{Buffer.data() + Buffer.size() - Layout.Size(), Layout};
	}

	Vertex Front()
	{
		assert(!Buffer.empty());
		return Vertex{Buffer.data(), Layout};
	}

	Vertex operator[](const size_t InIndex)
	{
		assert(InIndex < Size());
		return Vertex{Buffer.data() + Layout.Size() * InIndex, Layout};
	}
};