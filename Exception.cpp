#include "Exception.h"
#include <sstream>

char const* EngineException::what() const noexcept
{
	std::ostringstream StringStream;
	StringStream << GetType() << std::endl << GetOriginString();

	WhatBuffer = StringStream.str();
	return WhatBuffer.c_str();
}

std::string EngineException::GetOriginString() const noexcept
{
	std::ostringstream StringStream;
	StringStream << "[File] " << File << std::endl << "[Line] " << Line;
	return StringStream.str();
}

InfoException::InfoException(const int InLine, const char* InFile, const std::vector<std::string>& InInfoMessages) noexcept
	: EngineException(InLine, InFile)
{
	for (const auto& Message : InInfoMessages)
	{
		InfoMessage += Message;
		InfoMessage.push_back('\n');
	}

	if (!InfoMessage.empty())
	{
		InfoMessage.pop_back();
	}
}

const char* InfoException::what() const noexcept
{
	std::ostringstream StringStream;
	StringStream << GetType() << std::endl;

	if (!InfoMessage.empty())
	{
		StringStream << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	}

	StringStream << GetOriginString();

	WhatBuffer = StringStream.str();
	return WhatBuffer.c_str();
}

const char* ResultHandleException::what() const noexcept
{
	std::ostringstream StringStream;
	StringStream << GetType() << std::endl
				 << "[Error Code] " << GetErrorCode() << std::endl
				 << "[Description] " << GetErrorString();

	if (!InfoMessage.empty())
	{
		StringStream << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	}

	StringStream << GetOriginString();

	WhatBuffer = StringStream.str();
	return WhatBuffer.c_str();
}

std::string ResultHandleException::TranslateErrorCode(const HRESULT InResultHandle) noexcept
{
	char* MessageBuffer = nullptr;

	if (!FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
	                   nullptr, InResultHandle, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
	                   reinterpret_cast<LPSTR>(&MessageBuffer), 0, nullptr))
	{
		return {"Unidentified error code"};
	}

	std::string ErrorString = MessageBuffer;
	LocalFree(MessageBuffer);

	return ErrorString;
}

void ResultHandleException::Check(const int InLine, const char* InFile, const HRESULT InResultHandle,
                                  const std::vector<std::string>& InInfoMessages)
{
	if (FAILED(InResultHandle))
	{
#ifndef NDEBUG
		throw ResultHandleException{InLine, InFile, InResultHandle, InInfoMessages};
#else
		throw ResultHandleException{InLine, InFile, InResultHandle};
#endif
	}
}
