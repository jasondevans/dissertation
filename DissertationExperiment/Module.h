#pragma once

#include "stdafx.h"

class Module
{

public:

	virtual ~Module() {}

	virtual int render_frame() = 0;

	// Handle keyboard input.
	virtual LRESULT handleInput(RAWINPUT InputData) = 0;

	virtual int progressTimeSeconds();

};