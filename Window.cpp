#include "Window.h"
#include <sstream>
#define WINDOW_EXCEPTION(ResultHandle) WindowException(__LINE__, __FILE__, ResultHandle)
#define WINDOW_LAST_EXCEPTION() WindowException(__LINE__, __FILE__, GetLastError())

WindowClass::WindowClass() : HandleInstance(GetModuleHandle(nullptr))
{
	/**
	 * https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexw
	 * Window Class Configuration
	 */
	WNDCLASSEX WindowClass;
	WindowClass.cbSize = sizeof(WindowClass);
	WindowClass.style = CS_OWNDC;
	WindowClass.lpfnWndProc = Window::HandleMessageSetup;
	WindowClass.cbClsExtra = 0;
	WindowClass.cbWndExtra = 0;
	WindowClass.hInstance = GetHandleInstance();
	WindowClass.hIcon = nullptr;
	WindowClass.hCursor = nullptr;
	WindowClass.hbrBackground = nullptr;
	WindowClass.lpszMenuName = nullptr;
	WindowClass.lpszClassName = WindowClassName;
	WindowClass.hIconSm = nullptr;

	/**
	 * https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-registerclassexw
	 * Register Window Class
	 */
	RegisterClassEx(&WindowClass);
}

WindowClass::~WindowClass()
{
	UnregisterClass(WindowClassName, GetHandleInstance());
}

const wchar_t* WindowClass::GetName()
{
	return WindowClassName;
}

HINSTANCE WindowClass::GetHandleInstance() const
{
	return HandleInstance;
}

Window::Window(INT32 InWidth, INT32 InHeight, const wchar_t* InName)
{
	RECT WindowRect;
	WindowRect.left = 100;
	WindowRect.right = InWidth + WindowRect.left;
	WindowRect.top = 100;
	WindowRect.bottom = InHeight + WindowRect.top;

	if (FAILED(AdjustWindowRect(&WindowRect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE)))
	{
		throw WINDOW_LAST_EXCEPTION();
	}

	/**
	 * https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexa
	 * Create Window Instance
	 */
	Handle = CreateWindowEx(0, WindowClass::GetName(), InName, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, CW_USEDEFAULT,
	                        CW_USEDEFAULT, WindowRect.right - WindowRect.left, WindowRect.bottom - WindowRect.top,
	                        nullptr, nullptr, WindowClassInstance.GetHandleInstance(), this);

	if (!Handle)
	{
		throw WINDOW_LAST_EXCEPTION();
	}

	/**
	 * https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-showwindow
	 * Show Window
	 */
	ShowWindow(Handle, SW_SHOW);
}

Window::~Window()
{
	DestroyWindow(Handle);
}

LRESULT WINAPI Window::HandleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_NCCREATE)
	{
		const CREATESTRUCTW* const CreateStruct = reinterpret_cast<CREATESTRUCTW*>(lParam);
		const auto MyWindow = static_cast<Window*>(CreateStruct->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(MyWindow));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMessageThunk));
		return MyWindow->HandleMessage(hWnd, msg, wParam, lParam);
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI Window::HandleMessageThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	const auto MyWindow = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return MyWindow->HandleMessage(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
	// https://learn.microsoft.com/en-us/windows/win32/inputdev/wm-killfocus
	case WM_KILLFOCUS:
		{
			MyKeyboard.ClearState();
		}
		break;
	// https://learn.microsoft.com/en-us/windows/win32/inputdev/wm-keydown
	case WM_KEYDOWN: [[fallthrough]];
	// https://learn.microsoft.com/en-us/windows/win32/inputdev/wm-syskeydown
	case WM_SYSKEYDOWN:
		{
			// AutoRepeat가 활성화 되지 않았으면 꾹 누르고 있을 때 보내지는 메시지(30번 째 비트가 1)는 처리하지 않는다.
			if (!MyKeyboard.IsAutoRepeatEnabled() && lParam & 0x40000000) { break; }
			MyKeyboard.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;
	// https://learn.microsoft.com/en-us/windows/win32/inputdev/wm-keyup
	case WM_KEYUP: [[fallthrough]];
	// https://learn.microsoft.com/en-us/windows/win32/inputdev/wm-syskeyup
	case WM_SYSKEYUP:
		{
			MyKeyboard.OnKeyReleased(static_cast<unsigned char>(wParam));
		}
		break;
	// https://learn.microsoft.com/en-us/windows/win32/inputdev/wm-char
	case WM_CHAR:
		{
			MyKeyboard.OnChar(static_cast<unsigned char>(wParam));
		}
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

const char* WindowException::what() const noexcept
{
	std::ostringstream StringStream;
	StringStream << GetType() << std::endl
				 << "[Error Code] " << GetErrorCode() << std::endl
				 << "[Description] " << GetErrorString() << GetOriginString();

	WhatBuffer = StringStream.str();
	return WhatBuffer.c_str();
}

std::string WindowException::TranslateErrorCode(const HRESULT InResultHandle) noexcept
{
	char* MessageBuffer = nullptr;

	if (!FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
	                   nullptr, InResultHandle, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
	                   reinterpret_cast<LPSTR>(&MessageBuffer), 0, nullptr))
	{
		return {"Unidentified error code"};
	}

	std::string ErrorString = MessageBuffer;
	LocalFree(MessageBuffer);

	return ErrorString;
}