#pragma once

#include "stdafx.h"

// DirectX headers
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>

using namespace std;


class Color
{

public:

	// Red component.
	float r;

	// Green component.
	float g;

	// Blue component.
	float b;

	// Alpha component.
	float a;

	// Color implementation.
	D2D1::ColorF impl;

	// Constructor.
	Color();

	// Constructor.
	Color(int r, int g, int b);

	// Constructor.
	Color(int r, int g, int b, int a);

	// Constructor.
	Color(float r, float g, float b);

	// Constructor.
	Color(float r, float g, float b, float a);

	// Destructor.
	~Color();

};
