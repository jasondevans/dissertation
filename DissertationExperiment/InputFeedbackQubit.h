#pragma once

#include "Qubit.h"

using namespace std;

class InputFeedbackQubit : public Qubit
{
	public:

	// Text to display at the bottom of the screen.
	string bottomText;

	// Whether the subject has pressed an appropriate key.
	bool hasResponded;

	// The key that was pressed.
	string responseKey;

	// The number of frames to display the text for after response.
	int numFrames;

	// The number of post-response frames we've displayed so far.
	int numFramesDisplayed;

	// Constructor.
	InputFeedbackQubit(string bottomText);

	// Destructor.
	virtual ~InputFeedbackQubit();

	// Initialize just before first execution.
	virtual void liveInitialize();

	// Render a frame.
	virtual int render_frame();

	// Handle keyboard input.
	virtual LRESULT handleInput(RAWINPUT InputData);

};