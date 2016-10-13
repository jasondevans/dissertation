#pragma once

#include "Qubit.h"
#include "Color.h"

using namespace std;

class DisplayUntilKeypressQubit : public Qubit
{
	public:

	// The text to display.
	string displayText;

	// Text color.
	Color textColor;

	// Position to display the text.
	RECT position;

	// Keyboard states.
	bool anyKeyPressed;

	// The minimum number of frames to display the text for.
	int minFrames;

	// The number of frames we've displayed so far.
	int numFramesDisplayed;

	// Whether to show a progress indicator.
	bool showProgress;

	// Constructor.
	DisplayUntilKeypressQubit(const string& displayText);

	// Constructor.
	DisplayUntilKeypressQubit(const string& displayText, bool showProgress);

	// Constructor.
	DisplayUntilKeypressQubit(const string& displayText, int minFrames);

	// Constructor.
	DisplayUntilKeypressQubit(const string& displayText, int minFrames, bool showProgress);

	// Destructor.
	virtual ~DisplayUntilKeypressQubit();

	// Initialize just before first execution.
	virtual void liveInitialize();

	// Render a frame.
	virtual int render_frame();

	// Handle keyboard input.
	virtual LRESULT handleInput(RAWINPUT InputData);

private:

	// Do common initialization.
	void commonInitialization(const string& displayText, int minFrames, bool showProgress);

};