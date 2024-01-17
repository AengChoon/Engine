#include "Surface.h"
#define FULL_WINDOW
#include "EngineWin.h"
#include <cassert>
#include <gdiplus.h>
#include <sstream>
#include "ExceptionMacros.h"
#pragma comment(lib, "gdiplus.lib")

Surface::Surface(const unsigned InWidth, const unsigned InHeight) noexcept
	: Buffer(std::make_unique<Color[]>(static_cast<size_t>(InWidth * InHeight))), Width(InWidth), Height(InHeight)
{
}

Surface::Surface(const unsigned InWidth, const unsigned InHeight, std::unique_ptr<Color[]> InBuffer) noexcept
	: Buffer(std::move(InBuffer)), Width(InWidth), Height(InHeight)
{
}


Surface::Surface(Surface&& InSurface) noexcept
	: Buffer(std::move(InSurface.Buffer)), Width(InSurface.Width), Height(InSurface.Height)
{
}

Surface& Surface::operator=(Surface&& InSurface) noexcept
{
	Width = InSurface.Width;
	Height = InSurface.Height;
	Buffer = std::move(InSurface.Buffer);
	InSurface.Buffer = nullptr;
	return *this;
}

void Surface::Clear(const Color& InFillValue) const noexcept
{
	memset(Buffer.get(), InFillValue.ARGB, Width * Height * sizeof(Color));
}

void Surface::PutPixel(const unsigned InX, const unsigned InY, const Color& InColor)
{
	assert(InX < Width);
	assert(InY < Height);
	Buffer[InY * Width + InX] = InColor;
}

Surface::Color Surface::GetPixel(const unsigned InX, const unsigned InY) const
{
	assert(InX < Width);
	assert(InY < Height);
	return Buffer[InY * Width + InX];
}

unsigned Surface::GetWidth() const noexcept
{
	return Width;
}

unsigned Surface::GetHeight() const noexcept
{
	return Height;
}

Surface::Color* Surface::GetBufferPtr() noexcept
{
	return Buffer.get();
}

const Surface::Color* Surface::GetBufferPtrConst() const noexcept
{
	return Buffer.get();
}

Surface Surface::FromFile(const std::string& InFileName)
{
	unsigned int ImageWidth = 0;
	unsigned int ImageHeight = 0;
	std::unique_ptr<Color[]> ImageBuffer = nullptr;

	wchar_t WideFileName[512];
	mbstowcs_s(nullptr, WideFileName, InFileName.c_str(), _TRUNCATE);

	Gdiplus::Bitmap Bitmap {WideFileName};
	if (Bitmap.GetLastStatus() != Gdiplus::Status::Ok)
	{
		std::stringstream Stringstream;
		Stringstream << "Loading image [" << InFileName << "]: failed to load.";
		throw INFO_EXCEPTION({Stringstream.str()});
	}

	ImageWidth = Bitmap.GetWidth();
	ImageHeight = Bitmap.GetHeight();
	ImageBuffer = std::make_unique<Color[]>(static_cast<size_t>(ImageWidth * ImageHeight));

	for (unsigned int Row = 0; Row < ImageHeight; ++Row)
	{
		for (unsigned int Column = 0; Column < ImageWidth; ++Column)
		{
			Gdiplus::Color Color;
			Bitmap.GetPixel(static_cast<INT>(Column), static_cast<INT>(Row), &Color);
			ImageBuffer[Row * ImageWidth + Column] = Surface::Color{Color.GetValue()};
		}
	}

	return {ImageWidth, ImageHeight, std::move(ImageBuffer)};
}