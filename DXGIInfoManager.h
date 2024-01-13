#pragma once
#include <dxgidebug.h>
#include <string>
#include <vector>
#include "wrl/client.h"

class DXGIInfoManager
{
public:
	DXGIInfoManager();
	DXGIInfoManager(const DXGIInfoManager&) = delete;
	DXGIInfoManager(DXGIInfoManager&&) = delete;
	DXGIInfoManager& operator=(const DXGIInfoManager&) = delete;
	DXGIInfoManager& operator=(DXGIInfoManager&&) = delete;
	~DXGIInfoManager() = default;

	void Set() noexcept;
	[[nodiscard]] std::vector<std::string> GetMessages() const;

private:
	unsigned long long Next = 0u;
	Microsoft::WRL::ComPtr<IDXGIInfoQueue> DXGIInfoQueue;
};
