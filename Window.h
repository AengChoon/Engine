#pragma once
#include <memory>
#include "EngineWin.h"
#include "Graphics.h"
#include "Keyboard.h"
#include "Mouse.h"

class WindowClass
{
friend class Window;

public:
	WindowClass(const WindowClass&) = delete;
	WindowClass(WindowClass&&) = delete;
	WindowClass& operator=(const WindowClass&) = delete;
	WindowClass& operator=(WindowClass&&) = delete;

	static const wchar_t* GetName();
	HINSTANCE GetHandleInstance() const;

private:
	WindowClass();
	~WindowClass();

private:
	static constexpr const wchar_t* WindowClassName = L"Engine";

	HINSTANCE HandleInstance;
};

class Window
{
friend class WindowClass;

public:
	Window(int InWidth, int InHeight, const wchar_t* InName);
	Window(const Window&) = delete;
	Window(Window&&) = delete;
	Window& operator=(const Window&) = delete;
	Window& operator=(Window&&) = delete;
	~Window();

	static std::optional<int> ProcessMessages();

	Graphics& GetGraphics() const;

private:
	static LRESULT WINAPI HandleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI HandleMessageThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
	Keyboard MyKeyboard;
	Mouse MyMouse;

private:
	static inline WindowClass WindowClassInstance;

	HWND Handle;
	int Width;
	int Height;
	std::unique_ptr<Graphics> MyGraphics;
};