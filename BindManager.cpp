#include "BindManager.h"

BindManager& BindManager::Get()
{
	static BindManager MyBindManager;
	return MyBindManager;
}