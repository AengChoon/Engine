#include "Window.h"

// https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-winmain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	try
	{
		Window MyWindow {800, 300, WindowClass::GetName()};

		MSG Message;
		BOOL Result;

		while ((Result = GetMessage(&Message, nullptr, 0, 0)) > 0)
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}

		return Result == -1 ? -1 : static_cast<int>(Message.wParam);
	}
	catch (const EngineException& InException)
	{
		MessageBoxA(nullptr, InException.what(), InException.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& InException)
	{
		MessageBoxA(nullptr, InException.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBoxA(nullptr, "No Details Available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}

	return -1;
}