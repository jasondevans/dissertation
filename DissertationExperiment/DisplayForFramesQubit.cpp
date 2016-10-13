#include "stdafx.h"

#include "DisplayForFramesQubit.h"
#include "Util.h"
#include "Graphics.h"
#include "Color.h"

using namespace std;


// Constructor.
DisplayForFramesQubit::DisplayForFramesQubit(string displayText, string bottomText, int numFrames, Font* font)
{
	this->displayText = displayText;
	this->bottomText = bottomText;
	this->numFrames = numFrames;
	this->numFramesDisplayed = 0;
	this->font = font;
	this->color = Color(255, 255, 255, 255); // White by default
}


// Destructor.
DisplayForFramesQubit::~DisplayForFramesQubit()
{
}

// Initialize just before first execution.
void DisplayForFramesQubit::liveInitialize()
{
}


int DisplayForFramesQubit::render_frame()
{
	// Update priming timing variables, if appropriate.
	// (Turn on "can respond" if we're displaying a target word.)
	if (Util::getInstance().primingTimingEnabled && Util::getInstance().primingTimingCanRespond == false)
	{
		if ((std::find(Util::getInstance().firstGameWords.begin(), Util::getInstance().firstGameWords.end(), displayText) 
			!= Util::getInstance().firstGameWords.end() ? true : false) ||
			(std::find(Util::getInstance().secondGameWords.begin(), Util::getInstance().secondGameWords.end(), displayText) 
			!= Util::getInstance().secondGameWords.end() ? true : false))
		{
			Util::getInstance().primingTimingCanRespond = true;
		}

	}

	// Set up the frame.
	Graphics::getInstance().setUpNewFrame();

	// Display main text.
	Graphics::getInstance().drawTextCentered(displayText, font, color);

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
	numFramesDisplayed++;

	// Update priming timing stuff, if appropriate.
	if (Util::getInstance().primingTimingEnabled && Util::getInstance().primingTimingHasResponded)
	{
		Util::getInstance().primingTimingCountdownFrames--;
		if (Util::getInstance().primingTimingCountdownFrames == 0)
		{
			return Util::QUBIT_STATE_FINISHED;
		}
	}

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
LRESULT DisplayForFramesQubit::handleInput(RAWINPUT InputData)
{
	USHORT vKey = InputData.data.keyboard.VKey;
	bool isDown = (InputData.data.keyboard.Message == WM_KEYDOWN ? true : false);
	bool response = false;

	if (vKey == 0x46 && isDown && data["responseType"].compare("NR") == 0) // F
	{ 
		response = true;
		data["hasResponded"] = "true";
		data["responseKey"] = "f";
		data["response"] = Util::getInstance().randomization_priming_keymap ? "pleasant" : "unpleasant";
		if (data["exclamationPresented"].compare("true") == 0)
		{
			data["responseType"] = "LATE";
		}
		else
		{
			data["responseType"] = "EARLY";
		}
	}
	else if (vKey == 0x4A && isDown && data["responseType"].compare("NR") == 0) // J
	{
		response = true;
		data["hasResponded"] = "true";
		data["responseKey"] = "j";
		data["response"] = Util::getInstance().randomization_priming_keymap ? "unpleasant" : "pleasant";
		if (data["exclamationPresented"].compare("true") == 0)
		{
			data["responseType"] = "LATE";
		}
		else
		{
			data["responseType"] = "EARLY";
		}
	}

	// Update priming timing stuff, if applicable.
	if (response && Util::getInstance().primingTimingEnabled && Util::getInstance().primingTimingCanRespond
		&& Util::getInstance().primingTimingHasResponded == false)
	{
		Util::getInstance().primingTimingCountdownFrames = 36;
		Util::getInstance().primingTimingHasResponded = true;
	}

    return 0;
}



