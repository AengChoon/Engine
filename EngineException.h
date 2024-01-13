#pragma once
#include <exception>
#include <string>
#include <vector>
#include "DXGIInfoManager.h"
#include "EngineWin.h"

namespace Exception
{
	
}
class EngineException : public std::exception
{
public:
	EngineException(const int InLine, const char* InFile) noexcept
		: Line(InLine), File(InFile)
	{}

	char const* what() const noexcept override;

	virtual const char* GetType() const noexcept
	{
		return "Engine Exception";
	}

	int GetLine() const noexcept
	{
		return Line;
	}

	const std::string& GetFile() const noexcept
	{
		return File;
	}

	std::string GetOriginString() const noexcept;

protected:
	mutable std::string WhatBuffer;

private:
	int Line;
	std::string File;
};

class ResultHandleException final : public EngineException
{
public:
	ResultHandleException(int InLine, const char* InFile, const HRESULT InResultHandle, const std::vector<std::string>& InInfoMessages = {}) noexcept;

	const char* what() const noexcept override;

	const char* GetType() const noexcept override
	{
		return "Engine Window Exception";
	}

	static std::string TranslateErrorCode(HRESULT InResultHandle) noexcept;

	HRESULT GetErrorCode() const noexcept
	{
		return ResultHandle;
	}

	std::string GetErrorString() const noexcept
	{
		return TranslateErrorCode(ResultHandle);
	}

	const std::string& GetErrorInfo() const noexcept
	{
		return InfoMessage;
	}

	static void Check(int InLine, const char* InFile, HRESULT InResultHandle, const std::vector<std::string>& InInfoMessages = {});

private:
	HRESULT ResultHandle;
	std::string InfoMessage;
};