#pragma once

#include "Qubit.h"
#include "Color.h"

using namespace std;

class DisplayUntilFinishedQubit : public Qubit
{
	public:

	// The text to display.
	string displayText;

	// Text color.
	Color textColor;

	// Position to display the text.
	RECT position;

	// Keyboard states.
	bool finished;

	// Constructor.
	DisplayUntilFinishedQubit(const string& displayText);

	// Destructor.
	virtual ~DisplayUntilFinishedQubit();

	// Render a frame.
	virtual int render_frame();

	// Handle keyboard input.
	virtual LRESULT handleInput(RAWINPUT InputData);

};