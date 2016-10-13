#include "stdafx.h"

#include "ExecuteCodeModule.h"
#include "Util.h"
#include "Graphics.h"

using namespace std;

/* For now, this module only does one function -- finalizing and saving data files.
Theoretically, it could /should (per its name) be generalized to executing arbitrary code. */


// Constructor.
ExecuteCodeModule::ExecuteCodeModule()
{
}


// Destructor.
ExecuteCodeModule::~ExecuteCodeModule()
{
}


int ExecuteCodeModule::render_frame()
{
	// Execute our code (finalize and close data files).
	Util::getInstance().finishAndCloseFiles();

	// Display a blank frame.
	Graphics::getInstance().setUpNewFrame();
	Graphics::getInstance().finishAndPresentFrame();

	// This is execute once and return, so return that we're finished.
	return Util::MODULE_STATE_FINISHED;
}


// Handle keyboard input.
LRESULT ExecuteCodeModule::handleInput(RAWINPUT InputData)
{
    return 0;
}


int ExecuteCodeModule::progressTimeSeconds()
{
	return 1;
}

