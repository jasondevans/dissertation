#include "stdafx.h"

#include "StateManager.h"


// Constructor.
StateManager::StateManager()
{
	currentStateIndex = 0;
	hasBeenInitialized = false;
}


// Destructor.
StateManager::~StateManager()
{
}


// Execute one step.
int StateManager::executeStep()
{
	// Initialize if necessary.
	if (!hasBeenInitialized)
	{
		states[currentStateIndex]->initialize();
		hasBeenInitialized = true;
	}

	// Execute current step.
	int retVal = states[currentStateIndex]->executeStep();

	// Figure out what to do now.
	if (retVal == State::STATE_TRANSITION_NEXT)
	{
		// We finished the last state.
		if (currentStateIndex == states.size() - 1)
		{
			states[currentStateIndex]->cleanup();
			return StateManager::STATE_MANAGER_FINISHED;
		}
		// Move to the next state.
		else
		{
			states[currentStateIndex]->cleanup();
			currentStateIndex++;
			states[currentStateIndex]->initialize();
			return StateManager::STATE_MANAGER_RUNNING;
		}
	}
	else if (retVal == State::STATE_TRANSITION_PREV)
	{
		states[currentStateIndex]->cleanup();
		currentStateIndex = max(0, currentStateIndex - 1);
		states[currentStateIndex]->initialize();
		return StateManager::STATE_MANAGER_RUNNING;
	}
	else if (retVal == State::STATE_TRANSITION_FIRST)
	{
		states[currentStateIndex]->cleanup();
		currentStateIndex = 0;
		states[currentStateIndex]->initialize();
		return StateManager::STATE_MANAGER_RUNNING;
	}
	else // if (retVal == State::STATE_TRANSITION_KEEP_CURRENT)
	{
		// Do nothing.
		return StateManager::STATE_MANAGER_RUNNING;
	}
}


// Handle input.
LRESULT StateManager::handleInput(RAWINPUT rawInput)
{
	return states[currentStateIndex]->handleInput(rawInput);
}
