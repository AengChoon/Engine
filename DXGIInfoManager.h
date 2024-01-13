#pragma once
#include <string>
#include <vector>
#include "EngineWin.h"

struct IDXGIInfoQueue;

class DXGIInfoManager
{
public:
	DXGIInfoManager();
	DXGIInfoManager(const DXGIInfoManager&) = delete;
	DXGIInfoManager(DXGIInfoManager&&) = delete;
	DXGIInfoManager& operator=(const DXGIInfoManager&) = delete;
	DXGIInfoManager& operator=(DXGIInfoManager&&) = delete;
	~DXGIInfoManager();

	void Set() noexcept;
	[[nodiscard]] std::vector<std::string> GetMessages() const;

private:
	unsigned long long Next = 0u;
	IDXGIInfoQueue* DXGIInfoQueue = nullptr;
};
