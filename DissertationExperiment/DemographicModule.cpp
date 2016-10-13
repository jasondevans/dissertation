#include "stdafx.h"

#include "DemographicModule.h"
#include "Util.h"
#include "Graphics.h"
#include "Color.h"

using namespace std;


// Constructor.
DemographicModule::DemographicModule()
{
	// Set allowable characters.
	ageAllowable = "0123456789";
	genderAllowable = "abcdefghijklmnopqrstuvwxyz., '-0123456789";
	raceEthnicityAllowable = "abcdefghijklmnopqrstuvwxyz., '-0123456789";
	lhrhAllowable = "abcdefghijklmnopqrstuvwxyz., '-0123456789";

	// Set the initial state.
	state = STATE_AGE;

	// Set keyboard state.
	anyKeyPressed = false;
	shiftDown = false;
	key = "";
	age = "";
	gender = "";
	raceEthnicity = "";
	lhrh = "";

	// Set prompt text.
	instructions = "========== DEMOGRAPHIC INFORMATION ==========\n\n"
		"Please answer the following questions:\n\n";
	agePrompt = "Age:  ";
	genderPrompt = "Gender:  ";
	raceEthnicityPrompt = "Race / Ethnicity:  ";
	lhrhPrompt = "Do you consider yourself to be predominantly left-handed or right-handed?  ";
}


// Destructor.
DemographicModule::~DemographicModule()
{
}


int DemographicModule::render_frame()
{
	// Check for key press and process accordingly.
	if (anyKeyPressed)
	{
		if (state == STATE_AGE && string::npos != ageAllowable.find(key))
		{
			if (shiftDown) std::transform(key.begin(), key.end(), key.begin(), toupper);
			age += key;
		}
		else if (state == STATE_GENDER && string::npos != genderAllowable.find(key))
		{
			if (shiftDown) std::transform(key.begin(), key.end(), key.begin(), toupper);
			gender += key;
		}
		else if (state == STATE_RACEETHNICITY && string::npos != raceEthnicityAllowable.find(key))
		{
			if (shiftDown) std::transform(key.begin(), key.end(), key.begin(), toupper);
			raceEthnicity += key;
		}
		else if (state == STATE_LHRH && string::npos != lhrhAllowable.find(key))
		{
			if (shiftDown) std::transform(key.begin(), key.end(), key.begin(), toupper);
			lhrh += key;
		}

		anyKeyPressed = false;
	}

	// Build string to display.
	string wordText = instructions + agePrompt + age + (state == STATE_AGE ? "_" : "") + "\n\n" +
		genderPrompt + gender + (state == STATE_GENDER ? "_" : "") + "\n\n" +
		raceEthnicityPrompt + raceEthnicity + (state == STATE_RACEETHNICITY ? "_" : "") + "\n\n" +
		lhrhPrompt + lhrh + (state == STATE_LHRH ? "_" : "");

	// Word color.
	Color wordColor = Color(255, 255, 255, 255); // White

	// Draw the frame.
	Graphics::getInstance().setUpNewFrame();
	int positionX = Graphics::getInstance().displayWidth / 10;
	int positionY = Graphics::getInstance().displayHeight / 10;
	Graphics::getInstance().drawText(wordText, positionX, positionY, Graphics::getInstance().standardFont, wordColor);
	Graphics::getInstance().drawProgressText();
	Graphics::getInstance().finishAndPresentFrame();

	if (state == STATE_FINISHED)
	{
		// Write our data to the output file.
		Util::getInstance().outputSS << "<demographics>" << endl;
		Util::getInstance().outputSS << "<age>" << age << "</age>" << endl;
		Util::getInstance().outputSS << "<gender>" << gender << "</gender>" << endl;
		Util::getInstance().outputSS << "<race-ethnicity>" << raceEthnicity << "</race-ethnicity>" << endl;
		Util::getInstance().outputSS << "<handedness>" << lhrh << "</handedness>" << endl;
		Util::getInstance().outputSS << "</demographics>" << endl;

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
LRESULT DemographicModule::handleInput(RAWINPUT InputData)
{
	// Get input details.
	USHORT vKey = InputData.data.keyboard.VKey;
	bool isDown = (InputData.data.keyboard.Message == WM_KEYDOWN ? true : false);
	bool isUp = (InputData.data.keyboard.Message == WM_KEYUP ? true : false);

	// Process the shift key.
	if (vKey == VK_SHIFT && isDown) shiftDown = true;
	if (vKey == VK_SHIFT && isUp) shiftDown = false;

	// Process the backspace key.
	if (vKey == VK_BACK && isDown)
	{
		if (state == STATE_AGE) age = age.substr(0, max(0, (int) age.size() - 1));
		else if (state == STATE_GENDER) gender = gender.substr(0, max(0, (int) gender.size() - 1));
		else if (state == STATE_RACEETHNICITY) raceEthnicity = raceEthnicity.substr(0, max(0, (int) raceEthnicity.size() - 1));
		else if (state == STATE_LHRH) lhrh = lhrh.substr(0, max(0, (int) lhrh.size() - 1));
	}

	// Process the enter key.
	if (vKey == VK_RETURN && isDown)
	{
		if (state == STATE_AGE) { state = STATE_GENDER; }
		else if (state == STATE_GENDER) { state = STATE_RACEETHNICITY; }
		else if (state == STATE_RACEETHNICITY) { state = STATE_LHRH; }
		else if (state == STATE_LHRH) { state = STATE_FINISHED; }
	}

	// Process the tab and down arrow keys.
	if (((vKey == VK_TAB && !shiftDown) || vKey == VK_DOWN) && isDown)
	{
		if (state == STATE_AGE) { state = STATE_GENDER; }
		else if (state == STATE_GENDER) { state = STATE_RACEETHNICITY; }
		else if (state == STATE_RACEETHNICITY) { state = STATE_LHRH; }
	}

	// Process the shift-tab and up arrow keys.
	if (((vKey == VK_TAB && shiftDown) || vKey == VK_UP) && isDown)
	{
		if (state == STATE_GENDER) { state = STATE_AGE; }
		else if (state == STATE_RACEETHNICITY) { state = STATE_GENDER; }
		else if (state == STATE_LHRH) { state = STATE_RACEETHNICITY; }
	}

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


int DemographicModule::progressTimeSeconds()
{
	return 120;
}

