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

	static void Set() noexcept;
	[[nodiscard]] static std::vector<std::string> GetMessages();

private:
	static inline unsigned long long Next = 0u;
	static inline Microsoft::WRL::ComPtr<IDXGIInfoQueue> DXGIInfoQueue;
};
