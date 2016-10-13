#pragma once

#include "stdafx.h"

class Qubit
{

public:

	// Our public-viewable data.
	std::map<std::string, std::string> data;

	// Constructor.
	Qubit();

	// Destructor.
	virtual ~Qubit();

	// Initialize just before first execution.
	virtual void liveInitialize();

	// Render one frame.
	virtual int render_frame();

	// Handle keyboard input.
	virtual LRESULT handleInput(RAWINPUT InputData);

	// Perform any cleanup.
	virtual void cleanup();

};