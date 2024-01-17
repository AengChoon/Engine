#pragma once
#include "Exception.h"

#define ENGINE_EXCEPTION()																			\
	EngineException{__LINE__, __FILE__}

#define INFO_EXCEPTION(InMessages)																	\
	InfoException{__LINE__, __FILE__, InMessages}

#define CHECK_INFO_EXCEPTION(InFunction)															\
	DXGIInfoManager::Set();																			\
	(InFunction);																					\
	{																								\
		auto InfoMessages = DXGIInfoManager::GetMessages();										\
		if (!InfoMessages.empty())																	\
		{																							\
			throw INFO_EXCEPTION(InfoMessages);														\
		}																							\
	}

#define HRESULT_EXCEPTION(InResultHandle)															\
	ResultHandleException{__LINE__, __FILE__, (InResultHandle), DXGIInfoManager::GetMessages()}

#define HRESULT_LAST_EXCEPTION()																	\
	ResultHandleException{__LINE__, __FILE__, static_cast<HRESULT>(GetLastError())}

#define CHECK_HRESULT_EXCEPTION(InFunction)															\
	DXGIInfoManager::Set();																			\
	if (FAILED(ResultHandle = (InFunction)))														\
	{																								\
		throw HRESULT_EXCEPTION(ResultHandle);														\
	}								