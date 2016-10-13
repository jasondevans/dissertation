#pragma once

#include "stdafx.h"

using namespace std;

// DirectX headers
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>


class Font
{

public:

	// Size in points.
	float size;

	// Typeface.
	wstring typeface;

	// Reference to font implementation object.
	IDWriteTextFormat* impl;

	// Constructor.
	Font();

	// Constructor.
	Font(float size, wstring typeface);
	
	// Destructor.
	~Font();

};
