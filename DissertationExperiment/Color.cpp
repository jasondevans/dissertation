#include "stdafx.h"

#include "Color.h"
#include "Util.h"


// Constructor.
Color::Color() : impl(1.0f, 1.0f, 1.0f, 1.0f)
{
	this->r = 1.0f;
	this->g = 1.0f;
	this->b = 1.0f;
	this->a = 1.0f;
	this->impl = D2D1::ColorF(this->r, this->g, this->b, this->a);
}


// Constructor.
Color::Color(int r, int g, int b) : impl(1.0f, 1.0f, 1.0f, 1.0f)
{
	this->r = (float) r / 255.0f;
	this->g = (float) g / 255.0f;
	this->b = (float) b / 255.0f;
	this->a = 1.0f;
	this->impl = D2D1::ColorF(this->r, this->g, this->b, this->a);
}


// Constructor.
Color::Color(int r, int g, int b, int a) : impl(1.0f, 1.0f, 1.0f, 1.0f)
{
	this->r = (float) r / 255.0f;
	this->g = (float) g / 255.0f;
	this->b = (float) b / 255.0f;
	this->a = (float) a / 255.0f;
	this->impl = D2D1::ColorF(this->r, this->g, this->b, this->a);
}



// Constructor.
Color::Color(float r, float g, float b) : impl(1.0f, 1.0f, 1.0f, 1.0f)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = 1.0f;
	this->impl = D2D1::ColorF(this->r, this->g, this->b, this->a);
}


// Constructor.
Color::Color(float r, float g, float b, float a) : impl(1.0f, 1.0f, 1.0f, 1.0f)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
	this->impl = D2D1::ColorF(this->r, this->g, this->b, this->a);
}


// Destructor.
Color::~Color()
{
}






