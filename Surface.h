#pragma once
#include <memory>
#include <string>

class Surface
{
public:
	class Color
	{
	public:
		unsigned int ARGB;

	public:
		constexpr Color() noexcept : ARGB()
		{}

		constexpr Color(const Color& InColor) noexcept = default;

		explicit constexpr Color(const unsigned int InARGB) noexcept
			: ARGB(InARGB)
		{}

		constexpr Color(const unsigned char InA, const unsigned char InR,
						const unsigned char InG, const unsigned char InB) noexcept
			: ARGB(InA << 24u | InR << 16u | InG << 8u | InB)
		{}

		constexpr Color(const unsigned char InR, const unsigned char InG, const unsigned char InB) noexcept
			: ARGB(InR << 16u | InG << 8u | InB)
		{}

		constexpr Color(const Color InColor, const unsigned char InA) noexcept
			: Color(InA << 24u | InColor.ARGB)
		{}

		Color& operator=(const Color InColor) noexcept
		{
			ARGB = InColor.ARGB;
			return *this;
		}

		[[nodiscard]] constexpr unsigned char GetA() const noexcept
		{
			return ARGB >> 24u;
		}

		[[nodiscard]] constexpr unsigned char GetR() const noexcept
		{
			return (ARGB >> 16u) & 0xFFu;
		}

		[[nodiscard]] constexpr unsigned char GetG() const noexcept
		{
			return (ARGB >> 8u) & 0xFFu;
		}

		[[nodiscard]] constexpr unsigned char GetB() const noexcept
		{
			return ARGB & 0xFFu;
		}

		void SetA(const unsigned char InA) noexcept
		{
			ARGB = (ARGB & 0xFFFFFFu) | (InA << 24u);
		}

		void SetR(const unsigned char InR) noexcept
		{
			ARGB = (ARGB & 0xFF00FFFFu) | (InR << 16u);
		}

		void SetG(const unsigned char InG) noexcept
		{
			ARGB = (ARGB & 0xFFFF00FFu) | (InG << 8u);
		}

		void SetB(const unsigned char InB) noexcept
		{
			ARGB = (ARGB & 0xFFFFFF00u) | InB;
		}
	};

public:
	Surface(unsigned int InWidth, unsigned int InHeight) noexcept;
	Surface(Surface&& InSurface) noexcept;
	Surface(Surface&) = delete;
	Surface& operator=(Surface&& InSurface) noexcept;
	Surface& operator=(const Surface&) = delete;
	~Surface() = default;

	void Clear(const Color& InFillValue) const noexcept;
	void PutPixel(unsigned int InX, unsigned int InY, const Color& InColor);
	Color GetPixel(unsigned int InX, unsigned int InY) const;
	unsigned int GetWidth() const noexcept;
	unsigned int GetHeight() const noexcept;
	Color* GetBufferPtr() noexcept;
	const Color* GetBufferPtrConst() const noexcept;
	static Surface FromFile(const std::string& InFileName);


private:
	Surface(unsigned int InWidth, unsigned int InHeight, std::unique_ptr<Color[]> InBuffer) noexcept;

private:
	std::unique_ptr<Color[]> Buffer;
	unsigned int Width;
	unsigned int Height;
};
