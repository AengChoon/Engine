#include "Keyboard.h"

std::optional<Keyboard::Event> Keyboard::ReadKey() noexcept
{
	if (!KeyBuffer.empty())
	{
		const Event Event = KeyBuffer.front();
		KeyBuffer.pop();
		return std::optional{Event};
	}
	else
	{
		return std::nullopt;
	}
}

std::optional<char> Keyboard::ReadChar() noexcept
{
	if (!CharBuffer.empty())
	{
		const unsigned char CharCode = CharBuffer.front();
		CharBuffer.pop();
		return std::optional{CharCode};
	}
	else
	{
		return std::nullopt;
	}
}

void Keyboard::OnKeyPressed(unsigned char InKeyCode) noexcept
{
	KeyStates[InKeyCode] = true;
	KeyBuffer.emplace(Event::Type::Press, InKeyCode);
	TrimBuffer(KeyBuffer);
}

void Keyboard::OnKeyReleased(unsigned char InKeyCode) noexcept
{
	KeyStates[InKeyCode] = false;
	KeyBuffer.emplace(Event::Type::Release, InKeyCode);
	TrimBuffer(KeyBuffer);
}

void Keyboard::OnChar(unsigned char InCharacter) noexcept
{
	CharBuffer.emplace(InCharacter);
	TrimBuffer(CharBuffer);
}