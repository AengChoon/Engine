#pragma once

#define ENGINE_EXCEPTION()																			\
	EngineException{__LINE__, __FILE__}

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