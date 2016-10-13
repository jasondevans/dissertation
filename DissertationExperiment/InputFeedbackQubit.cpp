#include "stdafx.h"

#include "InputFeedbackQubit.h"
#include "Util.h"
#include "Graphics.h"
#include "Color.h"

using namespace std;


// Constructor.
InputFeedbackQubit::InputFeedbackQubit(string bottomText)
{
	this->bottomText = bottomText;
	hasResponded = false;
	responseKey = "";
	numFrames = 20;
	numFramesDisplayed = 0;
}


// Destructor.
InputFeedbackQubit::~InputFeedbackQubit()
{
}


// Initialize just before first execution.
void InputFeedbackQubit::liveInitialize()
{
}


int InputFeedbackQubit::render_frame()
{
	static string pleasantKey = Util::getInstance().randomization_priming_keymap ? "f" : "j";
	static string unpleasantKey = Util::getInstance().randomization_priming_keymap ? "j" : "f";
	
	string displayText = "";

	if (hasResponded)
	{
		data["responseType"] = "RESPONDED";
		if (data["targetIsPleasant"].compare("true") == 0 && responseKey.compare(pleasantKey) != 0
			|| data["targetIsPleasant"].compare("false") == 0 && responseKey.compare(unpleasantKey) != 0)
		{
			displayText = "ERROR";
		}
		else
		{
			return Util::QUBIT_STATE_FINISHED;
		}
	}

	// Word color.
	Color wordColor = Color(237, 28, 36, 255); // Red

	// Set up the frame.
	Graphics::getInstance().setUpNewFrame();

	// Display main text.
	Graphics::getInstance().drawTextCentered(displayText, Graphics::getInstance().wordFont, wordColor);

	// Display bottom text.
	RECT textDimensionsBottom = Graphics::getInstance().measureString(Graphics::getInstance().standardFont, bottomText);
	int wordWidth = textDimensionsBottom.right - textDimensionsBottom.left;
	int wordHeight = textDimensionsBottom.bottom - textDimensionsBottom.top;
	int positionX = (Graphics::getInstance().displayWidth - wordWidth) / 2;
	int positionY = Graphics::getInstance().displayHeight - wordHeight * 2;
	Graphics::getInstance().drawText(bottomText, positionX, positionY, Graphics::getInstance().standardFont, Color(255, 255, 255, 255));
	
	// Finish and present the frame.
	Graphics::getInstance().finishAndPresentFrame();

	// Update the number of frames we've displayed.
	if (hasResponded) numFramesDisplayed++;

	// Return appropriate state.
	if (numFramesDisplayed == numFrames)
	{
		return Util::QUBIT_STATE_FINISHED;
	}
	else
	{
		return Util::QUBIT_STATE_RUNNING;
	}
}


// Handle keyboard input.
LRESULT InputFeedbackQubit::handleInput(RAWINPUT InputData)
{
	USHORT vKey = InputData.data.keyboard.VKey;
	bool isDown = (InputData.data.keyboard.Message == WM_KEYDOWN ? true : false);
	bool isUp = (InputData.data.keyboard.Message == WM_KEYUP ? true : false);

	if (vKey == 0x46 && isDown) // F
	{ 
		hasResponded = true;
		responseKey = "f";
		data["responseKey"] = "f";
		data["response"] = Util::getInstance().randomization_priming_keymap ? "pleasant" : "unpleasant";
	}
	else if (vKey == 0x4A && isDown) // J
	{
		hasResponded = true;
		responseKey = "j";
		data["responseKey"] = "j";
		data["response"] = Util::getInstance().randomization_priming_keymap ? "unpleasant" : "pleasant";
	}

    return 0;
}



