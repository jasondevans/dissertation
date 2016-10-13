#include "stdafx.h"

#include "DisplayForFramesInputColorChangeQubit.h"
#include "Util.h"
#include "Color.h"

using namespace std;


// Constructor.
DisplayForFramesInputColorChangeQubit::DisplayForFramesInputColorChangeQubit(string displayText, string bottomText, int numFrames, Font* font)
	: DisplayForFramesQubit(displayText, bottomText, numFrames, font)
{
	hasResponded = false;
	responseKey = "";
}


// Destructor.
DisplayForFramesInputColorChangeQubit::~DisplayForFramesInputColorChangeQubit()
{
}


// Initialize just before first execution.
void DisplayForFramesInputColorChangeQubit::liveInitialize()
{
	if (data["responseType"].compare("EARLY") == 0) displayText = "";
	data["exclamationPresented"] = "true";
}


int DisplayForFramesInputColorChangeQubit::render_frame()
{
	// If subject early responded, skip this qubit altogether (for perceived timing).
	// if (data["responseType"].compare("EARLY") == 0) return Util::QUBIT_STATE_FINISHED;

	return DisplayForFramesQubit::render_frame();
}


// Handle keyboard input.
LRESULT DisplayForFramesInputColorChangeQubit::handleInput(RAWINPUT InputData)
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
		data["responseType"] = "WINDOW";
	}
	else if (vKey == 0x4A && isDown) // J
	{
		hasResponded = true;
		responseKey = "j";
		data["responseKey"] = "j";
		data["response"] = Util::getInstance().randomization_priming_keymap ? "unpleasant" : "pleasant";
		data["responseType"] = "WINDOW";
	}

	if (hasResponded) color = Color(237, 28, 36, 255); // Red

	// Update priming timing stuff, if applicable.
	if (hasResponded && Util::getInstance().primingTimingEnabled && Util::getInstance().primingTimingCanRespond
		&& Util::getInstance().primingTimingHasResponded == false)
	{
		Util::getInstance().primingTimingCountdownFrames = 36;
		Util::getInstance().primingTimingHasResponded = true;
	}

    return 0;
}



