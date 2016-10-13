#pragma once

#include "stdafx.h"
#include "State.h"

using namespace std;

class StateManager
{

public:

	// State manager status constants.
	static const int STATE_MANAGER_RUNNING = 2;
	static const int STATE_MANAGER_FINISHED = 3;

	// List of states.
	vector<State*> states;

	// Index of the current state.
	int currentStateIndex;

	// Whether state manager has been initialized.
	bool hasBeenInitialized;

	// Constructor.
	StateManager();

	// Destructor.
	virtual ~StateManager();

	// Execute one step.
	int executeStep();

	// Handle input.
	LRESULT handleInput(RAWINPUT rawInput);

};