#pragma once

#include "Qubit.h"
#include "DisplayForFramesQubit.h"
#include "Font.h"

using namespace std;

class DisplayForFramesInputColorChangeQubit : public DisplayForFramesQubit
{
	public:

	// Whether the subject has pressed an appropriate key.
	bool hasResponded;

	// The key that was pressed.
	string responseKey;

	// Constructor.
	DisplayForFramesInputColorChangeQubit(string displayText, string bottomText, int numFrames, Font* font);

	// Destructor.
	virtual ~DisplayForFramesInputColorChangeQubit();

	// Initialize just before first execution.
	virtual void liveInitialize();

	// Render a frame.
	virtual int render_frame();

	// Handle keyboard input.
	virtual LRESULT handleInput(RAWINPUT InputData);

};