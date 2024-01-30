#include "Window.h"
#include <sstream>
#include "Exception.h"
#include "ExceptionMacros.h"
#include "imgui/imgui_impl_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

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

Window::Window(const int InWidth, const int InHeight, const wchar_t* InName)
	: Width(InWidth), Height(InHeight)
{
	RECT WindowRect;
	WindowRect.left = 0;
	WindowRect.right = InWidth + WindowRect.left;
	WindowRect.top = 0;
	WindowRect.bottom = InHeight + WindowRect.top;

	if (!AdjustWindowRect(&WindowRect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE))
	{
		throw HRESULT_LAST_EXCEPTION();
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
		throw HRESULT_LAST_EXCEPTION();
	}

	/**
	 * https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-showwindow
	 * Show Window
	 */
	ShowWindow(Handle, SW_SHOW);

	RAWINPUTDEVICE RawInputDevice;
	RawInputDevice.usUsagePage = 0x01;
	RawInputDevice.usUsage = 0x02;
	RawInputDevice.dwFlags = 0;
	RawInputDevice.hwndTarget = nullptr;

	if (RegisterRawInputDevices(&RawInputDevice, 1, sizeof(RawInputDevice)) == FALSE)
	{
		throw HRESULT_LAST_EXCEPTION();
	}

	ImGui_ImplWin32_Init(Handle);
	MyGraphics = std::make_unique<Graphics>(Handle, InWidth, InHeight);
}

Window::~Window()
{
	ImGui_ImplWin32_Shutdown();
	DestroyWindow(Handle);
}

std::optional<int> Window::ProcessMessages()
{
	MSG Message;

	while (PeekMessage(&Message, nullptr, 0, 0, PM_REMOVE))
	{
		if (Message.message == WM_QUIT)
		{
			return std::optional{static_cast<int>(Message.wParam)};
		}

		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return std::nullopt;
}

Graphics& Window::GetGraphics() const
{
	if (!MyGraphics)
	{
		throw ENGINE_EXCEPTION();
	}

	return *MyGraphics;
}

void Window::EnableCursor()
{
	bIsCursorEnabled = true;
	ShowCursor();
	EnableImGuiMouseInput();
	FreeCursor();
}

void Window::DisableCursor()
{
	bIsCursorEnabled = false;
	HideCursor();
	DisableImGuiMouseInput();
	TrapCursor();
}

bool Window::IsCursorEnabled()
{
	return bIsCursorEnabled;
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
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
	{
		return true;
	}

	const auto& ImGuiIO = ImGui::GetIO();

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
	case WM_ACTIVATE:
		{
			 if (!bIsCursorEnabled)
			 {
			 	if (wParam & WA_ACTIVE)
			 	{
			 		TrapCursor();
			 		HideCursor();
			 	}
			 	else
			 	{
			 		FreeCursor();
			 		ShowCursor();
			 	}
			 }
		}
		break;
	// https://learn.microsoft.com/en-us/windows/win32/inputdev/wm-keydown
	case WM_KEYDOWN: [[fallthrough]];
	// https://learn.microsoft.com/en-us/windows/win32/inputdev/wm-syskeydown
	case WM_SYSKEYDOWN:
		{
			if (ImGuiIO.WantCaptureKeyboard) { break; }

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
			if (ImGuiIO.WantCaptureKeyboard) { break; }
			MyKeyboard.OnKeyReleased(static_cast<unsigned char>(wParam));
		}
		break;
	// https://learn.microsoft.com/en-us/windows/win32/inputdev/wm-char
	case WM_CHAR:
		{
			if (ImGuiIO.WantCaptureKeyboard) { break; }
			MyKeyboard.OnChar(static_cast<unsigned char>(wParam));
		}
		break;
	case WM_MOUSEMOVE:
		{
			const auto [X, Y] = MAKEPOINTS(lParam);

			if (!bIsCursorEnabled)
			{
				if (!MyMouse.IsInWindow())
				{
					SetCapture(hWnd);
					MyMouse.OnEnter();
					HideCursor();
				}

				break;
			}

			if (ImGuiIO.WantCaptureMouse) { break; }

			if (X >= 0 && X < Width && Y >= 0 && Y < Height)
			{
				MyMouse.OnMove(X, Y);

				if (!MyMouse.IsInWindow())
				{
					SetCapture(hWnd);
					MyMouse.OnEnter();
				}
			}
			else
			{
				if (MyMouse.IsLeftPressed() || MyMouse.IsRightPressed())
				{
					MyMouse.OnMove(X, Y);
				}
				else
				{
					ReleaseCapture();
					MyMouse.OnLeave();
				}
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			SetForegroundWindow(hWnd);

			if (!bIsCursorEnabled)
			{
				TrapCursor();
				HideCursor();
			}

			if (ImGuiIO.WantCaptureMouse) { break; }
			const auto [X, Y] = MAKEPOINTS(lParam);
			MyMouse.OnLeftPressed(X, Y);
		}
		break;
	case WM_RBUTTONDOWN:
		{
			if (ImGuiIO.WantCaptureMouse) { break; }
			const auto [X, Y] = MAKEPOINTS(lParam);
			MyMouse.OnRightPressed(X, Y);
		}
		break;
	case WM_LBUTTONUP:
		{
			if (ImGuiIO.WantCaptureMouse) { break; }
			const auto [X, Y] = MAKEPOINTS(lParam);
			MyMouse.OnLeftReleased(X, Y);

			if (X < 0 || X >= Width || Y < 0 || Y >= Height)
			{
				ReleaseCapture();
				MyMouse.OnLeave();
			}
		}
		break;
	case WM_RBUTTONUP:
		{
			if (ImGuiIO.WantCaptureMouse) { break; }
			const auto [X, Y] = MAKEPOINTS(lParam);
			MyMouse.OnRightReleased(X, Y);

			if (X < 0 || X >= Width || Y < 0 || Y >= Height)
			{
				ReleaseCapture();
				MyMouse.OnLeave();
			}
		}
		break;
	case WM_MOUSEWHEEL:
		{
			if (ImGuiIO.WantCaptureMouse) { break; }
			const auto [X, Y] = MAKEPOINTS(lParam);
			MyMouse.OnWheelDelta(X, Y, GET_WHEEL_DELTA_WPARAM(wParam));
		}
		break;
	case WM_INPUT:
		{
			if (!MyMouse.IsRawInputEnabled()) { break; }

			UINT Size;

			if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, nullptr, &Size, sizeof(RAWINPUTHEADER)) == -1)
			{
				break;
			}

			RawBuffer.resize(Size);

			if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, RawBuffer.data(), &Size, sizeof(RAWINPUTHEADER)) != Size)
			{
				break;
			}

			if (const auto& [Header, Data] = reinterpret_cast<const RAWINPUT&>(*RawBuffer.data()); Header.dwType == RIM_TYPEMOUSE && (Data.mouse.lLastX != 0 || Data.mouse.lLastY != 0))
			{
				MyMouse.OnRawDelta(Data.mouse.lLastX, Data.mouse.lLastY);
			}
		}
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void Window::ShowCursor()
{
	while (::ShowCursor(TRUE) < 0);
}

void Window::HideCursor()
{
	while (::ShowCursor(FALSE) >= 0);
}

void Window::TrapCursor()
{
	RECT ClipRect;
	GetClientRect(Handle, &ClipRect);
	MapWindowPoints(Handle, nullptr, reinterpret_cast<LPPOINT>(&ClipRect), 2);
	ClipCursor(&ClipRect);
}

void Window::FreeCursor()
{
	ClipCursor(nullptr);
}

void Window::EnableImGuiMouseInput()
{
	ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
}

void Window::DisableImGuiMouseInput()
{
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
}
