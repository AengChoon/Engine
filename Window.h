#pragma once
#include "EngineWin.h"
#include "EngineException.h"

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
	Window(INT32 InWidth, INT32 InHeight, const wchar_t* InName);
	Window(const Window&) = delete;
	Window(Window&&) = delete;
	Window& operator=(const Window&) = delete;
	Window& operator=(Window&&) = delete;
	~Window();

private:
	static LRESULT WINAPI HandleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI HandleMessageThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	static inline WindowClass WindowClassInstance;

	HWND Handle;
	INT32 Width;
	INT32 Height;
};

#define WINDOW_EXCEPTION(ResultHandle) WindowException(__LINE__, __FILE__, ResultHandle)
#define WINDOW_LAST_EXCEPTION() WindowException(__LINE__, __FILE__, GetLastError())
class WindowException : public EngineException
{
public:
	WindowException(const int InLine, const char* InFile, const HRESULT InResultHandle) noexcept
		: EngineException(InLine, InFile), ResultHandle(InResultHandle)
	{}

	const char* what() const noexcept override;

	const char* GetType() const noexcept override
	{
		return "Engine Window Exception";
	}

	static std::string TranslateErrorCode(HRESULT InResultHandle) noexcept;

	HRESULT GetErrorCode() const noexcept
	{
		return ResultHandle;
	}

	std::string GetErrorString() const noexcept
	{
		return TranslateErrorCode(ResultHandle);
	}

private:
	HRESULT ResultHandle;
};