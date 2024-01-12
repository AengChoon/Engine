#pragma once
#include <queue>
#include <optional>

class Mouse
{
	friend class Window;

public:
	class Event
	{
	public:
		enum class Type
		{
			LeftPress,
			LeftRelease,
			RightPress,
			RightRelease,
			WheelUp,
			WheelDown,
			Move,
			Enter,
			Leave,
			Invalid
		};

		Event() noexcept
			: Type(Type::Invalid), bIsLeftPressed(false), bIsRightPressed(false), X(0), Y(0)
		{}

		Event(const Type InType, const Mouse& InMouse) noexcept
			: Type(InType), bIsLeftPressed(InMouse.bIsLeftPressed), bIsRightPressed(InMouse.bIsRightPressed),
			  X(InMouse.X), Y(InMouse.Y)
		{}

		[[nodiscard]] bool IsValid() const noexcept
		{
			return Type != Type::Invalid;
		}

		[[nodiscard]] Type GetType() const noexcept
		{
			return Type;
		}

		[[nodiscard]] std::pair<int, int> GetPos() const noexcept
		{
			return {X, Y};
		}

		[[nodiscard]] int GetPosX() const noexcept
		{
			return X;
		}

		[[nodiscard]] int GetPosY() const noexcept
		{
			return Y;
		}

		[[nodiscard]] bool IsLeftPressed() const noexcept
		{
			return bIsLeftPressed;
		}

		[[nodiscard]] bool IsRightPressed() const noexcept
		{
			return bIsRightPressed;
		}

	private:
		Type Type;
		bool bIsLeftPressed;
		bool bIsRightPressed;
		int X;
		int Y;
	};

	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse(Mouse&&) = delete;
	Mouse& operator=(const Mouse&) = delete;
	Mouse& operator=(Mouse&&) = delete;
	~Mouse() = default;

	std::optional<Event> Read() noexcept;

	void Flush() noexcept
	{
		Buffer = std::queue<Event>{};
	}

	[[nodiscard]] std::pair<int, int> GetPos() const noexcept
	{
		return {X, Y};
	}

	[[nodiscard]] int GetPosX() const noexcept
	{
		return X;
	}

	[[nodiscard]] int GetPosY() const noexcept
	{
		return Y;
	}

	[[nodiscard]] bool IsLeftPressed() const noexcept
	{
		return bIsLeftPressed;
	}

	[[nodiscard]] bool IsRightPressed() const noexcept
	{
		return bIsRightPressed;
	}

	[[nodiscard]] bool IsInWindow() const noexcept
	{
		return bIsInWindow;
	}

	[[nodiscard]] bool IsEmpty() const noexcept
	{
		return Buffer.empty();
	}

private:
	void OnMouseMove(int InX, int InY) noexcept;
	void OnLeftPressed(int InX, int InY) noexcept;
	void OnLeftReleased(int InX, int InY) noexcept;
	void OnRightPressed(int InX, int InY) noexcept;
	void OnRightReleased(int InX, int InY) noexcept;
	void OnWheelUp(int InX, int InY) noexcept;
	void OnWheelDown(int InX, int InY) noexcept;
	void OnWheelDelta(int InX, int InY, int InDelta);
	void OnMouseLeave() noexcept;
	void OnMouseEnter() noexcept;
	void TrimBuffer() noexcept;

private:
	static constexpr unsigned int BufferSize {16u};

	int X;
	int Y;
	int WheelDeltaCarry;
	bool bIsLeftPressed {false};
	bool bIsRightPressed {false};
	bool bIsInWindow {false};

	std::queue<Event> Buffer;
};