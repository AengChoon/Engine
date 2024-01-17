#pragma once
#define FULL_WINDOW
#include "EngineWin.h"

class GDIPlusManager
{
public:
	GDIPlusManager();
	GDIPlusManager(const GDIPlusManager&) = delete;
	GDIPlusManager(GDIPlusManager&&) = delete;
	GDIPlusManager& operator=(const GDIPlusManager&) = delete;
	GDIPlusManager& operator=(GDIPlusManager&&) = delete;
	~GDIPlusManager();

private:
	static inline ULONG_PTR Token {0};
	static inline int RefCount {0};
};
