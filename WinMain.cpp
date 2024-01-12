#include "App.h"

// https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-winmain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	try
	{
		return App{}.Run();
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