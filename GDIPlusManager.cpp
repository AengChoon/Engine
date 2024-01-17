#include "GDIPlusManager.h"
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

GDIPlusManager::GDIPlusManager()
{
	if (RefCount++ == 0)
	{
		Gdiplus::GdiplusStartupInput StartupInput;
		StartupInput.GdiplusVersion = 1;
		StartupInput.DebugEventCallback = nullptr;
		StartupInput.SuppressBackgroundThread = false;
		Gdiplus::GdiplusStartup(&Token, &StartupInput, nullptr);
	}
}

GDIPlusManager::~GDIPlusManager()
{
	if (--RefCount == 0)
	{
		Gdiplus::GdiplusShutdown(Token);
	}
}