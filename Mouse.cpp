#include "Mouse.h"
#include "EngineWin.h"

std::optional<Mouse::Event> Mouse::Read() noexcept
{
	if (!Buffer.empty())
	{
		const auto Event = Buffer.front();
		Buffer.pop();
		return std::optional{Event};
	}
	else
	{
		return std::nullopt;
	}
}

void Mouse::OnMouseMove(int InX, int InY) noexcept
{
	X = InX;
	Y = InY;

	Buffer.emplace(Event::Type::Move, *this);
	TrimBuffer();
}

void Mouse::OnLeftPressed(int InX, int InY) noexcept
{
	bIsLeftPressed = true;

	Buffer.emplace(Event::Type::LeftPress, *this);
	TrimBuffer();
}

void Mouse::OnLeftReleased(int InX, int InY) noexcept
{
	bIsLeftPressed = false;

	Buffer.emplace(Event::Type::LeftRelease, *this);
	TrimBuffer();
}

void Mouse::OnRightPressed(int InX, int InY) noexcept
{
	bIsRightPressed = true;

	Buffer.emplace(Event::Type::RightPress, *this);
	TrimBuffer();
}

void Mouse::OnRightReleased(int InX, int InY) noexcept
{
	bIsRightPressed = false;

	Buffer.emplace(Event::Type::RightRelease, *this);
	TrimBuffer();
}

void Mouse::OnWheelUp(int InX, int InY) noexcept
{
	Buffer.emplace(Event::Type::WheelUp, *this);
	TrimBuffer();
}

void Mouse::OnWheelDown(int InX, int InY) noexcept
{
	Buffer.emplace(Event::Type::WheelDown, *this);
	TrimBuffer();
}

void Mouse::OnWheelDelta(int InX, int InY, int InDelta)
{
	WheelDeltaCarry += InDelta;

	while (WheelDeltaCarry >= WHEEL_DELTA)
	{
		WheelDeltaCarry -= WHEEL_DELTA;
		OnWheelUp(InX, InY);
	}

	while (WheelDeltaCarry <= -WHEEL_DELTA)
	{
		WheelDeltaCarry += WHEEL_DELTA;
		OnWheelDown(InX, InY);
	}
}

void Mouse::OnMouseLeave() noexcept
{
	bIsInWindow = false;

	Buffer.emplace(Event::Type::Leave, *this);
	TrimBuffer();
}

void Mouse::OnMouseEnter() noexcept
{
	bIsInWindow = true;

	Buffer.emplace(Event::Type::Enter, *this);
	TrimBuffer();
}

void Mouse::TrimBuffer() noexcept
{
	while (Buffer.size() > BufferSize)
	{
		Buffer.pop();
	}
}