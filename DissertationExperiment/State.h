#pragma once

#include "stdafx.h"

#include "Qubit.h"

using namespace std;

class State
{

public:

	// State transition constants.
	static const int STATE_TRANSITION_KEEP_CURRENT = 1;
	static const int STATE_TRANSITION_NEXT = 2;
	static const int STATE_TRANSITION_PREV = 3;
	static const int STATE_TRANSITION_FIRST = 4;

	// Our execute step function.
	boost::function<int ()> executeStepFn;

	// Our handle input function.
	boost::function<LRESULT (RAWINPUT)> handleInputFn;

	// Our initialization function.
	boost::function<void ()> initializeFn;

	// Our cleanup function.
	boost::function<void ()> cleanupFn;

	// Constructor.
	State();

	// Constructor.
	State(boost::function<int ()> executeStepFn, boost::function<LRESULT (RAWINPUT)> handleInputFn);

	// Constructor.
	State(Qubit& qubit);

	// Destructor.
	virtual ~State();

	// Execute one step.
	int executeStep();

	// Handle input.
	LRESULT handleInput(RAWINPUT rawInput);

	// Initialize.
	void initialize();

	// Clean up.
	void cleanup();

};