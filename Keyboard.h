#pragma once
#include <queue>
#include <bitset>
#include <optional>

class Keyboard
{
	friend class Window;

public:
	class Event
	{
		public:
			enum class Type
			{
				Press,
				Release,
				Invalid
			};

			Event() noexcept
				: Type(Type::Invalid), Code(0u)
			{}

			Event(const Type InType, const unsigned char InCode) noexcept
				: Type(InType), Code(InCode)
			{}

			[[nodiscard]] bool IsPress() const noexcept
			{
				return Type == Type::Press;
			}

			[[nodiscard]] bool IsRelease() const noexcept
			{
				return Type == Type::Release;
			}

			[[nodiscard]] bool IsValid() const noexcept
			{
				return Type != Type::Invalid;
			}

			[[nodiscard]] unsigned char GetCode() const noexcept
			{
				return Code;
			}

		private:
			Type Type;
			unsigned char Code;
	};

	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard(Keyboard&&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;
	Keyboard& operator=(Keyboard&&) = delete;
	~Keyboard() = default;

	std::optional<Event> ReadKey() noexcept;
	std::optional<char> ReadChar() noexcept;

	void FlushKey() noexcept
	{
		KeyBuffer = std::queue<Event>{};
	}

	void FlushChar() noexcept
	{
		CharBuffer = std::queue<unsigned char>{};
	}

	void Flush() noexcept
	{
		FlushKey();
		FlushChar();
	}

	void EnableAutoRepeat() noexcept
	{
		bIsAutoRepeatEnabled = true;
	}

	void DisableAutoRepeat() noexcept
	{
		bIsAutoRepeatEnabled = false;
	}

	[[nodiscard]] bool IsKeyPressed(const unsigned char InKeyCode) const noexcept
	{
		return KeyStates[InKeyCode];
	}

	[[nodiscard]] bool IsKeyEmpty() const noexcept
	{
		return KeyBuffer.empty();
	}

	[[nodiscard]] bool IsCharEmpty() const noexcept
	{
		return CharBuffer.empty();
	}

	[[nodiscard]] bool IsAutoRepeatEnabled() const noexcept
	{
		return bIsAutoRepeatEnabled;
	}

private:
	template<typename T>
	static void TrimBuffer(std::queue<T>& InBuffer) noexcept
	{
		while (InBuffer.size() > BufferSize)
		{
			InBuffer.pop();
		}
	}

	void OnKeyPressed(unsigned char InKeyCode) noexcept;
	void OnKeyReleased(unsigned char InKeyCode) noexcept;
	void OnChar(unsigned char InCharacter) noexcept;

	void ClearState() noexcept
	{
		KeyStates.reset();
	}

private:
	static constexpr unsigned int KeyNum {256u};
	static constexpr unsigned int BufferSize {16u};

	bool bIsAutoRepeatEnabled = false;
	std::bitset<KeyNum> KeyStates;
	std::queue<Event> KeyBuffer;
	std::queue<unsigned char> CharBuffer;
};