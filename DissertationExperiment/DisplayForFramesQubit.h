#pragma once

#include "Qubit.h"
#include "Font.h"
#include "Color.h"

using namespace std;

class DisplayForFramesQubit : public Qubit
{
	public:

	// The text to display.
	string displayText;

	// Text to display at the bottom of the screen.
	string bottomText;

	// The number of frames to display the text for.
	int numFrames;

	// The number of frames we've displayed so far.
	int numFramesDisplayed;

	// The font to use.
	Font* font;

	// The color to use.
	Color color;

	// Constructor.
	DisplayForFramesQubit(string displayText, string bottomText, int numFrames, Font* font);

	// Destructor.
	virtual ~DisplayForFramesQubit();

	// Initialize just before first execution.
	virtual void liveInitialize();

	// Render a frame.
	virtual int render_frame();

	// Handle keyboard input.
	virtual LRESULT handleInput(RAWINPUT InputData);

};