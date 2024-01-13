#include "DXGIInfoManager.h"
#include <memory>
#include "EngineException.h"
#pragma comment(lib, "dxguid.lib")

DXGIInfoManager::DXGIInfoManager()
{
	using DXGIGetDebugInterface = HRESULT(WINAPI*)(REFIID, void**);

	const auto ModuleHandle = LoadLibraryEx(L"dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
	if (!ModuleHandle)
	{
		throw ResultHandleException(__LINE__, __FILE__, GetLastError());
	}

	const auto DXGIDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(reinterpret_cast<void*>(GetProcAddress(ModuleHandle, "DXGIGetDebugInterface")));
	if (!DXGIDebugInterface)
	{
		throw ResultHandleException(__LINE__, __FILE__, GetLastError());
	}
	
	if (const auto ResultHandle = DXGIDebugInterface(__uuidof(IDXGIInfoQueue), &DXGIInfoQueue); FAILED(ResultHandle))
	{
		throw ResultHandleException(__LINE__, __FILE__, ResultHandle);
	}
}

void DXGIInfoManager::Set() noexcept
{
	Next = DXGIInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
}

std::vector<std::string> DXGIInfoManager::GetMessages() const
{
	std::vector<std::string> Messages;
	const auto End = DXGIInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);

	for(auto Index = Next; Index < End; Index++)
	{
		size_t MessageLength;

		if (const auto ResultHandle = DXGIInfoQueue->GetMessage(DXGI_DEBUG_ALL, Index, nullptr, &MessageLength); FAILED(ResultHandle))
		{
			throw ResultHandleException(__LINE__, __FILE__, ResultHandle);
		}

		auto Bytes = std::make_unique<byte[]>(MessageLength);
		const auto MessageBuffer = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(Bytes.get());

		if (const auto ResultHandle = DXGIInfoQueue->GetMessage(DXGI_DEBUG_ALL, Index, MessageBuffer, &MessageLength); FAILED(ResultHandle))
		{
			throw ResultHandleException(__LINE__, __FILE__, ResultHandle);
		}

		Messages.emplace_back(MessageBuffer->pDescription);
	}

	return Messages;
}
