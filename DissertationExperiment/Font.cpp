#include "stdafx.h"

#include "Font.h"
#include "Graphics.h"


// Constructor.
Font::Font()
{
	size = 0.0f;
	typeface = L"";
	impl = NULL;
}


// Constructor.
Font::Font(float size, wstring typeface)
{
	this->size = size;
	this->typeface = typeface;

	Graphics::getInstance().dwriteFactory->CreateTextFormat(
		typeface.c_str(),
		NULL,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		size,
		L"en-us",
		&impl);
	impl->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	impl->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	/*
	// Create the implementation object.
	D3DXCreateFont(Graphics::getInstance().d3ddev,    // the Direct3D Device
        size, 0,    // font size in points with the default width
        FW_NORMAL,    // normal font weight
        1,    // no mipmap levels
        FALSE,    // not italic
        DEFAULT_CHARSET,    // default character set
        OUT_DEFAULT_PRECIS,    // default precision
        // DEFAULT_QUALITY,    // default quality
		CLEARTYPE_QUALITY,
        DEFAULT_PITCH || FF_DONTCARE,    // more defaults...
        typeface.c_str(),    // typeface "Arial"
        &impl);    // address of our font object
	*/
}


// Destructor.
Font::~Font()
{
}





