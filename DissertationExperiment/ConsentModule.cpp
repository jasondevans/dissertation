#include "stdafx.h"

#include "ConsentModule.h"
#include "Util.h"
#include "Graphics.h"
#include "Color.h"

using namespace std;


// Constructor.
ConsentModule::ConsentModule()
{
	// Set allowable keys (others are ignored).
	allowableKeys = "abcdefghijklmnopqrstuvwxyz1234567890' .,";

	// Set the initial state.
	state = IN_PROGRESS;

	// Set keyboard state.
	anyKeyPressed = false;
	shiftDown = false;
	key = "";
	subjectName = "";

	string instrHeader = "========== WELCOME AND CONSENT FORM ==========\n\n";
	consentText = instrHeader + Util::getInstance().insertNewLines(""
		"Thank you for participating! In this study, you will answer "
		"some questions about yourself, play a computer "
		"video game, and engage in other interactive computer tasks.", 86) + "\n\n"
		"" + Util::getInstance().insertNewLines(""
		"Your responses and the data collected will be kept "
		"confidential and your name will not be associated with any data that you "
		"provide. You may decline to participate, or end your participation at any "
		"point without penalty. After the study, you will be fully debriefed about "
		"the purpose of the research.", 86) + "\n\n" + Util::getInstance().insertNewLines(""
		"You may request a copy of this consent form for your records. "
		"If you have any questions or concerns regarding your participation in "
		"this experiment, please contact Jason Evans, the principal investigator, "
		"at XXX-XXX-XXXX or email@domain.edu. If you do not receive a "
		"satisfactory response, or if you have any questions about your rights as "
		"a volunteer, you may contact the New School's Institutional Review Board "
		"Coordinator at email@domain.edu.", 86) + "\n\n" + Util::getInstance().insertNewLines(""
		">>> Feel free to take breaks to rest, "
		"stretch, etc. as needed between sections. Also note that the experiment uses only "
		"the keyboard -- no mouse is used.", 86) + "\n\n" +
		Util::getInstance().insertNewLines(""
		"By typing your name below, you acknowledge that you have read and understood the "
		"procedures to be followed and consent to participation in this study, and "
		"you confirm that you are least 18 years of age. (Please type your name below and "
		"hit \"Enter\"):", 86) + "\n\n"
		"Name:  ";
}


// Destructor.
ConsentModule::~ConsentModule()
{
}


int ConsentModule::render_frame()
{
	// Check for key press and update state.
	if (anyKeyPressed)
	{
		if (string::npos != allowableKeys.find(key))
		{
			if (shiftDown) std::transform(key.begin(), key.end(), key.begin(), toupper);
			subjectName += key;
		}
		anyKeyPressed = false;
	}

	// Figure out what to display.
	string wordText = consentText + subjectName + "_";

	// Word color.
	Color wordColor = Color(255, 255, 255, 255); // White

	// Set up the new frame.
	Graphics::getInstance().setUpNewFrame();

	// Draw the text.
	Graphics::getInstance().drawTextCentered(wordText, Graphics::getInstance().standardFont, wordColor);

	// Draw progress text.
	Graphics::getInstance().drawProgressText();

	// Finish and present frame.
	Graphics::getInstance().finishAndPresentFrame();

	if (state == FINISHED)
	{
		// Get the current date and time, in UTC.
		SYSTEMTIME st;
		GetSystemTime(&st);

		// Write our data to the output file.
		Util::getInstance().consentSS << "<consent>" << endl;
		Util::getInstance().consentSS << "<name>" << subjectName << "</name>" << endl;
		Util::getInstance().consentSS << "<datetime-utc>" << endl;
		Util::getInstance().consentSS << "<year>" << st.wYear << "</year>" << endl;
		Util::getInstance().consentSS << "<month>" << st.wMonth << "</month>" << endl;
		Util::getInstance().consentSS << "<day>" << st.wDay << "</day>" << endl;
		Util::getInstance().consentSS << "<hour>" << st.wHour << "</hour>" << endl;
		Util::getInstance().consentSS << "<minute>" << st.wMinute << "</minute>" << endl;
		Util::getInstance().consentSS << "<second>" << st.wSecond << "</second>" << endl;
		Util::getInstance().consentSS << "</datetime-utc>" << endl;
		Util::getInstance().consentSS << "</consent>" << endl;

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
LRESULT ConsentModule::handleInput(RAWINPUT InputData)
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
		subjectName = subjectName.substr(0, max(0, (int) subjectName.size() - 1));
	}

	// Process the enter key.
	if (vKey == VK_RETURN)
	{
		state = FINISHED;
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


int ConsentModule::progressTimeSeconds()
{
	return 60;
}


