#pragma once
#include <exception>
#include <string>

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