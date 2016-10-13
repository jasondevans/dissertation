// Include shared header file.
#include "stdafx.h"

// Include our header file.
#include "UploadModule.h"
#include "DisplayUntilKeypressQubit.h"


// Constructor.
UploadModule::UploadModule()
{
	// Our instructions.
	string instructions = Util::getInstance().insertNewLines(""
		"You have completed the main part of the experiment. Your results "
		"will now be uploaded to the server.", 80) + "\n\n" +
		"Press any key to continue ---------->" + "\n\n" + Util::getInstance().insertNewLines(""
		"(This may take up to a minute -- after pressing a key, "
		"you won't see any changes to this screen until the upload completes.)", 80);

	// Instructions qubit.
	pInstructionsQubit = new DisplayUntilKeypressQubit(instructions, 50 /* 3 seconds */, true);

	// Add instructions state.
	State* instructionsState = new State(*pInstructionsQubit);
	// instructionsState->initializeFn = boost::bind(&Util::finishAndCloseFiles, boost::ref(Util::getInstance()));
	stateManager.states.push_back(instructionsState);

	// Upload finished qubit.
	pUploadFinishedQubit = new DisplayUntilKeypressQubit("Upload finished!  Press any key to continue.", 50 /* 3 seconds */);

	// Add upload finished state.
	State* uploadFinishedState = new State(*pUploadFinishedQubit);
	uploadFinishedState->initializeFn = boost::bind(&UploadModule::uploadFiles, boost::ref(*this));
	stateManager.states.push_back(uploadFinishedState);
}


int UploadModule::render_frame()
{
	int retVal = stateManager.executeStep();
	if (retVal == StateManager::STATE_MANAGER_FINISHED) return Util::MODULE_STATE_FINISHED;
	else return Util::MODULE_STATE_RUNNING;
}


// Destructor.
UploadModule::~UploadModule()
{
	delete pInstructionsQubit;

	delete pUploadFinishedQubit;

	for (auto iter = stateManager.states.begin(); iter != stateManager.states.end(); ++iter)
	{
		delete (*iter);
	}
}


// Handle keyboard input.
LRESULT UploadModule::handleInput(RAWINPUT InputData)
{
	return stateManager.handleInput(InputData);
}



void UploadModule::uploadFiles()
{
	// Upload our data file.
	Util::getInstance().uploadFile(Util::getInstance().dataFileDirectory + L"\\" + Util::getInstance().dataFileName,
		L"/data/" + Util::getInstance().dataFileName);

	// Upload consent file.
	Util::getInstance().uploadFile(Util::getInstance().dataFileDirectory + L"\\" + Util::getInstance().consentFileName,
		L"/consent/" + Util::getInstance().consentFileName);
}


int UploadModule::progressTimeSeconds()
{
	return 60;
}
