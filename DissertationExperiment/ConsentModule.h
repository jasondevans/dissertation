#pragma once

#include "Module.h"

using namespace std;

class ConsentModule : public Module
{
	public:

	// Current state of the module.
	int state;

	// Keyboard states.
	bool anyKeyPressed;
	string key;
	bool shiftDown;

	// String containing allowable keys for user to type (others are ignored).
	string allowableKeys;

	// Consent text.
	string consentText;

	// Subject name.
	string subjectName;

	// State constants.
	static const int IN_PROGRESS = 1;  // In progress.
	static const int FINISHED = 2;  // We're done.

	// Constructor.
	ConsentModule();

	// Destructor.
	virtual ~ConsentModule();

	virtual int render_frame();

	// Handle keyboard input.
	virtual LRESULT handleInput(RAWINPUT InputData);

	virtual int progressTimeSeconds();

};