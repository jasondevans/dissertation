#include "stdafx.h"

#include "DebriefingModule.h"
#include "Util.h"
#include "Graphics.h"
#include "Color.h"

using namespace std;


// Constructor.
DebriefingModule::DebriefingModule()
{
	// Set the initial state.
	state = STATE_IN_PROGRESS;

	// Set keyboard state.
	anyKeyPressed = false;
	shiftDown = false;
	key = "";

	// Set prompt text.
	text = ""
		"Thank you for participating in this experiment. The purpose of this\n"
		"study is to investigate whether \"approach\" and \"avoidance\" behaviors\n"
		"can influence the way people feel about the things approached or\n"
		"avoided, causing them to have a more positive or a more negative\n"
		"attitude.\n\n"
		"Drawing upon \"Eastern\" practices such as meditation and mindfulness, we\n"
		"hypothesize that avoiding things may actually make you feel more\n"
		"negatively toward them, and that approaching things may make you feel\n"
		"more positively about them.\n\n"
		"In this experiment, we are attempting to measure this in a concrete way\n"
		"by having you approach and avoid words in a video game.  The word\n"
		"categorization involved presentation of the words you had seen in the\n"
		"game very briefly, so that in most cases you could not see them.  This\n"
		"was intended to influence your ability to categorize the visible words,\n"
		"as an indirect measure of the way you felt about the words.\n\n"
		"If our hypothesis is supported in this experiment, it may give us a\n"
		"better idea of how these Eastern practices \"work\", and in turn may\n"
		"contribute to developing methods for helping people to feel better, and\n"
		"suffer less.\n\n"
		"(Press any key to end the experiment.)";
}


// Destructor.
DebriefingModule::~DebriefingModule()
{
}


int DebriefingModule::render_frame()
{
	// Check for key press and process accordingly.
	if (anyKeyPressed)
	{
		state = STATE_FINISHED;
		anyKeyPressed = false;
	}

	// Draw the frame.
	Color wordColor = Color(255, 255, 255, 255); // White
	Graphics::getInstance().setUpNewFrame();
	Graphics::getInstance().drawTextCentered(text, Graphics::getInstance().standardFont, wordColor);
	Graphics::getInstance().drawProgressText();
	Graphics::getInstance().finishAndPresentFrame();

	if (state == STATE_FINISHED)
	{
		// Return finished state.
		return Util::MODULE_STATE_FINISHED;
	}
	else
	{
		// Return still running state.
		return Util::MODULE_STATE_RUNNING;
	}
}


// Handle keyboard input.
LRESULT DebriefingModule::handleInput(RAWINPUT InputData)
{
	// Get input details.
	USHORT vKey = InputData.data.keyboard.VKey;
	bool isDown = (InputData.data.keyboard.Message == WM_KEYDOWN ? true : false);
	bool isUp = (InputData.data.keyboard.Message == WM_KEYUP ? true : false);

	// Register key press.
	if (isDown) 
	{
		anyKeyPressed = true;
		WORD result = 0;
		byte keyboardState[256];
		GetKeyboardState(keyboardState);
		ToAscii(vKey, InputData.data.keyboard.MakeCode, keyboardState, &result, 0);
		key = (char) result; 
	}

	// We successfully handled the input.
    return 0;
}


int DebriefingModule::progressTimeSeconds()
{
	return 60;
}


