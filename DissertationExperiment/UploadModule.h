#pragma once

#include "Util.h"
#include "Module.h"
#include "Qubit.h"
#include "DisplayUntilFinishedQubit.h"
#include "StateManager.h"


class UploadModule : public Module
{

public:

	// (Pointer to) qubit for displaying instructions.
	Qubit* pInstructionsQubit;

	// (Pointer to) qubit for displaying "upload finished".
	Qubit* pUploadFinishedQubit;

	// Our state manager.
	StateManager stateManager;

	// Constructor
	UploadModule();

	// Destructor.
	virtual ~UploadModule();

	// Render a frame.
	virtual int render_frame();

	// Handle keyboard input.
	virtual LRESULT handleInput(RAWINPUT InputData);

	virtual int progressTimeSeconds();

private:

	virtual void uploadFiles();

};