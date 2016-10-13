#pragma once

#include "Module.h"

using namespace std;

class DebriefingModule : public Module
{
	public:

	// Current state of the module.
	int state;

	// Keyboard states.
	bool anyKeyPressed;
	string key;
	bool shiftDown;

	// Prompt text.
	string text;

	// State constants.
	static const int STATE_IN_PROGRESS = 0;  // We're in progress.
	static const int STATE_FINISHED = 1;  // We're done.

	// Constructor.
	DebriefingModule();

	// Destructor.
	virtual ~DebriefingModule();

	virtual int render_frame();

	// Handle keyboard input.
	virtual LRESULT handleInput(RAWINPUT InputData);

	virtual int progressTimeSeconds();

};