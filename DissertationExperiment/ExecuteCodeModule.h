#pragma once

#include "Module.h"

using namespace std;

/* For now, this module only does one function -- finalizing and saving data files.
Theoretically, it could /should (per its name) be generalized to executing arbitrary code. */

class ExecuteCodeModule : public Module
{

public:

	// Whether this QModule has been initialized.
	bool initialized;

	// Current state of the module.
	int state;

	// Keyboard states.
	bool anyKeyPressed;
	string key;

	// State constants.
	static const int START = 0;  // At the beginning.
	static const int QUESTIONS = 1;  // Doing questions.
	static const int FINISHED = 2;  // We're done.

	// Constructor.
	ExecuteCodeModule();

	// Destructor.
	virtual ~ExecuteCodeModule();

	virtual int render_frame();

	// Handle keyboard input.
	virtual LRESULT handleInput(RAWINPUT InputData);

	virtual int progressTimeSeconds();

};