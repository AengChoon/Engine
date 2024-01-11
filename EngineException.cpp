#include "EngineException.h"
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