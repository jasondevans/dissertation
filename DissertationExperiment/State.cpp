#include "stdafx.h"

#include "State.h"


// Constructor.
State::State()
{
}


// Constructor.
State::State(boost::function<int ()> executeStepFn, boost::function<LRESULT (RAWINPUT)> handleInputFn)
{
	this->executeStepFn = executeStepFn;
	this->handleInputFn = handleInputFn;
}


// Constructor.
State::State(Qubit& qubit)
{
	this->executeStepFn = boost::bind(&Qubit::render_frame, boost::ref(qubit));
	this->handleInputFn = boost::bind(&Qubit::handleInput, boost::ref(qubit), _1);
}


// Destructor.
State::~State()
{
}


// Execute one step.
int State::executeStep()
{
	return executeStepFn();
}


// Handle input.
LRESULT State::handleInput(RAWINPUT rawInput)
{
	return handleInputFn(rawInput);
}


// Initialize.
void State::initialize()
{
	if (initializeFn) initializeFn();
}


// Clean up.
void State::cleanup()
{
	if (cleanupFn) cleanupFn();
}


