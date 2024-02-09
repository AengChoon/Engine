#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>
#include <vector>

namespace DV
{
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
			Tangent,
			Bitangent,
			Float3Color,
			Float4Color,
			BGRAColor
		};

		template<ElementType> struct TypeMap;

		template<> struct TypeMap<ElementType::Position2D>
		{
			using SystemType = DirectX::XMFLOAT2;
			static constexpr DXGI_FORMAT DxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
			static constexpr const char* Semantic = "Position";
			static constexpr const char* Code = "P2";
		};

		template<> struct TypeMap<ElementType::Position3D>
		{
			using SystemType = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT DxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* Semantic = "Position";
			static constexpr const char* Code = "P3";
		};

		template<> struct TypeMap<ElementType::Texture2D>
		{
			using SystemType = DirectX::XMFLOAT2;
			static constexpr DXGI_FORMAT DxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
			static constexpr const char* Semantic = "TexCoord";
			static constexpr const char* Code = "T2";
		};

		template<> struct TypeMap<ElementType::Normal>
		{
			using SystemType = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT DxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* Semantic = "Normal";
			static constexpr const char* Code = "N";
		};

		template<> struct TypeMap<ElementType::Tangent>
		{
			using SystemType = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT DxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* Semantic = "Tangent";
			static constexpr const char* Code = "Nt";
		};

		template<> struct TypeMap<ElementType::Bitangent>
		{
			using SystemType = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT DxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* Semantic = "Bitangent";
			static constexpr const char* Code = "Nbt";
		};

		template<> struct TypeMap<ElementType::Float3Color>
		{
			using SystemType = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT DxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* Semantic = "Color";
			static constexpr const char* Code = "RGB";
		};

		template<> struct TypeMap<ElementType::Float4Color>
		{
			using SystemType = DirectX::XMFLOAT4;
			static constexpr DXGI_FORMAT DxgiFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
			static constexpr const char* Semantic = "Color";
			static constexpr const char* Code = "RGBA32";
		};

		template<> struct TypeMap<ElementType::BGRAColor>
		{
			using SystemType = BGRAColor;
			static constexpr DXGI_FORMAT DxgiFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
			static constexpr const char* Semantic = "Color";
			static constexpr const char* Code = "RGBA8";
		};

		class Element
		{
		private:
			ElementType Type;
			size_t ByteOffset;

		public:
			Element(const ElementType InType, const size_t InByteOffset)
				: Type(InType), ByteOffset(InByteOffset)
			{}

			[[nodiscard]] D3D11_INPUT_ELEMENT_DESC GetDesc() const
			{
				switch (Type)
				{
				case ElementType::Position2D:
					return GenerateDesc<ElementType::Position2D>(GetByteOffset());
				case ElementType::Position3D:
					return GenerateDesc<ElementType::Position3D>(GetByteOffset());
				case ElementType::Texture2D:
					return GenerateDesc<ElementType::Texture2D>(GetByteOffset());
				case ElementType::Normal:
					return GenerateDesc<ElementType::Normal>(GetByteOffset());
				case ElementType::Tangent:
					return GenerateDesc<ElementType::Tangent>(GetByteOffset());
				case ElementType::Bitangent:
					return GenerateDesc<ElementType::Bitangent>(GetByteOffset());
				case ElementType::Float3Color:
					return GenerateDesc<ElementType::Float3Color>(GetByteOffset());;
				case ElementType::Float4Color:
					return GenerateDesc<ElementType::Float4Color>(GetByteOffset());
				case ElementType::BGRAColor:
					return GenerateDesc<ElementType::BGRAColor>(GetByteOffset());
				default:
					assert(false && "Invalid element type");
					return {"INVALID", 0, DXGI_FORMAT_UNKNOWN, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0};
				}
			}

			[[nodiscard]] ElementType GetType() const
			{
				return Type;
			}

			[[nodiscard]] size_t GetByteOffset() const
			{
				return ByteOffset;
			}

			[[nodiscard]] const char* GetCode() const noexcept
			{
				switch (Type)
				{
				case ElementType::Position2D:
					return TypeMap<ElementType::Position2D>::Code;
				case ElementType::Position3D:
					return TypeMap<ElementType::Position3D>::Code;
				case ElementType::Texture2D:
					return TypeMap<ElementType::Texture2D>::Code;
				case ElementType::Normal:	
					return TypeMap<ElementType::Normal>::Code;
				case ElementType::Tangent:	
					return TypeMap<ElementType::Tangent>::Code;
				case ElementType::Bitangent:	
					return TypeMap<ElementType::Bitangent>::Code;
				case ElementType::Float3Color:
					return TypeMap<ElementType::Float3Color>::Code;
				case ElementType::Float4Color:	
					return TypeMap<ElementType::Float4Color>::Code;
				case ElementType::BGRAColor:
					return TypeMap<ElementType::BGRAColor>::Code;
				default:
					assert(false && "Invalid Element Type");
					return nullptr;
				}
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
					return sizeof(TypeMap<ElementType::Position2D>::SystemType);
				case ElementType::Position3D:
					return sizeof(TypeMap<ElementType::Position3D>::SystemType);
				case ElementType::Texture2D:
					return sizeof(TypeMap<ElementType::Texture2D>::SystemType);
				case ElementType::Normal:
					return sizeof(TypeMap<ElementType::Normal>::SystemType);
				case ElementType::Tangent:
					return sizeof(TypeMap<ElementType::Tangent>::SystemType);
				case ElementType::Bitangent:
					return sizeof(TypeMap<ElementType::Bitangent>::SystemType);
				case ElementType::Float3Color:
					return sizeof(TypeMap<ElementType::Float3Color>::SystemType);
				case ElementType::Float4Color:
					return sizeof(TypeMap<ElementType::Float4Color>::SystemType);
				case ElementType::BGRAColor:
					return sizeof(TypeMap<ElementType::BGRAColor>::SystemType);
				default:
					assert(false && "Invalid Element Type");
					return NULL;
				}
			}

		private:
			template<ElementType T>
			static constexpr D3D11_INPUT_ELEMENT_DESC GenerateDesc(const size_t InOffset) noexcept
			{
				using ElementType = TypeMap<T>;
				return {ElementType::Semantic,0, ElementType::DxgiFormat,
						   0, static_cast<UINT>(InOffset),
				 		   D3D11_INPUT_PER_VERTEX_DATA, 0};
			}
		};

	private:
		std::vector<Element> Elements;

	public:
		VertexLayout(const std::initializer_list<ElementType> InElementTypeList)
		{
			for (const auto ElementType : InElementTypeList)
			{
				Elements.emplace_back(ElementType, Size());
			}
		}

		VertexLayout& Append(ElementType InElementType)
		{
			Elements.emplace_back(InElementType, Size());
			return *this;
		}

		template<ElementType T>
		[[nodiscard]] const Element& Resolve() const
		{
			if (auto Result = std::find_if(Elements.begin(), Elements.end(), [](const Element& InElement)
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

		[[nodiscard]] std::vector<D3D11_INPUT_ELEMENT_DESC> GetD3D11Layout() const
		{
			std::vector<D3D11_INPUT_ELEMENT_DESC> InputElementDescs;
			InputElementDescs.reserve(Num());

			for (const auto& Element : Elements)
			{
				InputElementDescs.push_back(Element.GetDesc());
			}

			return InputElementDescs;
		}

		[[nodiscard]] std::string GetCode() const
		{
			std::string AccumulatedCode;

			for (const auto& Element : Elements)
			{
				AccumulatedCode += Element.GetCode();
			}

			return AccumulatedCode;
		}

		[[nodiscard]] const Element& ResolveByIndex(const size_t InIndex) const
		{
			return Elements[InIndex];
		}

		[[nodiscard]] size_t Size() const
		{
			if (!Elements.empty())
			{
				const auto& LastElement = Elements.back();
				return LastElement.GetByteOffset() + LastElement.Size();
			}
			else
			{
				return NULL;
			}
		}

		[[nodiscard]] size_t Num() const
		{
			return Elements.size();
		}
	};

	class Vertex
	{
		friend class VertexBuffer;

	private:
		byte* Data {nullptr};
		std::reference_wrapper<const VertexLayout> Layout;

	public:
		template<VertexLayout::ElementType ElementType>
		auto& Attribute()
		{
			auto AttributeAddress = Data + Layout.get().Resolve<ElementType>().GetByteOffset();
			return *reinterpret_cast<typename VertexLayout::TypeMap<ElementType>::SystemType*>(AttributeAddress);
		}

		template<typename T>
		void SetAttributeByIndex(const size_t InIndex, T&& InValue)
		{
			const auto& Element = Layout.get().ResolveByIndex(InIndex);
			auto Attribute = Data + Element.GetByteOffset();

			switch (Element.GetType())
			{
			case VertexLayout::ElementType::Position2D:
				SetAttribute<VertexLayout::ElementType::Position2D>(Attribute, std::forward<T>(InValue));
				break;
			case VertexLayout::ElementType::Position3D:
				SetAttribute<VertexLayout::ElementType::Position3D>(Attribute, std::forward<T>(InValue));
				break;
			case VertexLayout::ElementType::Texture2D:
				SetAttribute<VertexLayout::ElementType::Texture2D>(Attribute, std::forward<T>(InValue));
				break;
			case VertexLayout::ElementType::Normal:
				SetAttribute<VertexLayout::ElementType::Normal>(Attribute, std::forward<T>(InValue));
				break;
			case VertexLayout::ElementType::Tangent:
				SetAttribute<VertexLayout::ElementType::Tangent>(Attribute, std::forward<T>(InValue));
				break;
			case VertexLayout::ElementType::Bitangent:
				SetAttribute<VertexLayout::ElementType::Bitangent>(Attribute, std::forward<T>(InValue));
				break;
			case VertexLayout::ElementType::Float3Color:
				SetAttribute<VertexLayout::ElementType::Float3Color>(Attribute, std::forward<T>(InValue));
				break;
			case VertexLayout::ElementType::Float4Color:
				SetAttribute<VertexLayout::ElementType::Float4Color>(Attribute, std::forward<T>(InValue));
				break;
			case VertexLayout::ElementType::BGRAColor:
				SetAttribute<VertexLayout::ElementType::BGRAColor>(Attribute, std::forward<T>(InValue));
				break;
			default: assert(false && "Invalid Element Type");
			}
		}

	protected:
		Vertex(byte* InData, const VertexLayout& InLayout)
			: Data(InData), Layout(InLayout)
		{
			assert(InData);
		}

	private:
		template<VertexLayout::ElementType Destination, typename Source>
		void SetAttribute(byte* InAttribute, Source&& InValue) 
		{
			using DestinationSystemType = typename VertexLayout::TypeMap<Destination>::SystemType;

			if constexpr (std::is_assignable_v<DestinationSystemType, Source>)
			{
				*reinterpret_cast<DestinationSystemType*>(InAttribute) = InValue;
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
		std::vector<byte> Buffer;
		VertexLayout Layout;

	public:
		explicit VertexBuffer(VertexLayout&& InLayout)
			: Layout(std::move(InLayout))
		{}

		[[nodiscard]] const byte* GetData() const
		{
			return Buffer.data();
		}

		[[nodiscard]] const VertexLayout& GetLayout() const
		{
			return Layout;
		}

		[[nodiscard]] size_t Num() const
		{
			return Buffer.size() / Layout.Size();
		}

		[[nodiscard]] size_t Size() const
		{
			return Buffer.size();
		}

		template<typename ...Params>
		void Emplace(Params&&... InParams)
		{
			assert(sizeof...(InParams) == Layout.Num() && "Parameter count doesn't match vertex count");
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
			assert(InIndex < Num());
			return Vertex{Buffer.data() + Layout.Size() * InIndex, Layout};
		}
	};
}