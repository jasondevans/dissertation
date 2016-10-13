#include "stdafx.h"

#include "QModule.h"
#include "Util.h"
#include "Graphics.h"
#include "Color.h"

using namespace std;


// Constructor.
QModule::QModule()
{
	state = START;
	anyKeyPressed = false;
	key = "";
	initialized = false;
}


// Constructor.
QModule::QModule(string startInstructions, string everyScreenInstructions, vector<string> questions, 
	vector<string> questionInfos, string allowableResponses, string questionnaireName)
{
	this->startInstructions = startInstructions;
	this->everyScreenInstructions = everyScreenInstructions;
	this->questions = questions;
	this->questionInfos = questionInfos;
	this->allowableResponses = allowableResponses;
	this->questionnaireName = questionnaireName;

	state = START;
	anyKeyPressed = false;
	key = "";
	initialized = false;
}


// Destructor.
QModule::~QModule()
{
}


int QModule::render_frame()
{
	// Initialize, if we haven't already.
	if (!initialized)
	{
		string instrHeader = "========== QUESTIONNAIRE ==========\n\n";
		string instrFooter = "\n\n\nPress any key to continue >>>>>";
		startInstructions = instrHeader + startInstructions + instrFooter;
		initialized = true;
	}

	// Word color.
	Color wordColor = Color(255, 255, 255, 255); // White by default

	// Clear the window.
	Graphics::getInstance().setUpNewFrame();

	// Figure out what to display based on our state, and update state.
	string wordText = "";
	if (state == START)
	{
		wordText = startInstructions;
	}
	else if (state == QUESTIONS)
	{
		stringstream sstm;
		sstm << questions[currentQ] << "\n\n" << everyScreenInstructions;
		wordText = sstm.str();
	}

	// Display the text.
	Graphics::getInstance().drawTextCentered(wordText, Graphics::getInstance().standardFont, wordColor);

	// Potentially display a progress indicator.
	if (state == START)
	{
		Graphics::getInstance().drawProgressText();
	}

	// Finish and present the frame.
	Graphics::getInstance().finishAndPresentFrame();

	// Check for key press and update state.
	if (anyKeyPressed)
	{
		if (state == START)
		{
			state = QUESTIONS;
			currentQ = 0;
		}
		else if (state == QUESTIONS && string::npos != allowableResponses.find(key))
		{
			responses.push_back(key);
			if ((int) (questions.size() - 1) > currentQ) currentQ++;
			else state = FINISHED;
		}
		anyKeyPressed = false;
	}

	if (state == FINISHED)
	{
		// Write our data to file.
		Util::getInstance().outputSS << "<questionnaire name=\"" << questionnaireName << "\">" << endl;
		for (unsigned int i = 0; i < responses.size(); i++)
		{
			Util::getInstance().outputSS << "<question num=\"" << i << "\" info=\"" << questionInfos[i] << "\">" 
				<< responses[i] << "</question>" << endl;
		}
		Util::getInstance().outputSS << "</questionnaire>" << endl;

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
LRESULT QModule::handleInput(RAWINPUT InputData)
{
	USHORT vKey = InputData.data.keyboard.VKey;
	bool isDown = (InputData.data.keyboard.Message == WM_KEYDOWN ? true : false);
	bool isUp = (InputData.data.keyboard.Message == WM_KEYUP ? true : false);

	if (isDown) 
	{
		anyKeyPressed = true;
		WORD result = 0;
		byte keyboardState[256];
		GetKeyboardState(keyboardState);
		ToAscii(vKey, InputData.data.keyboard.MakeCode, keyboardState, &result, 0);
		key = (char) result; 
	}

    return 0;
}


int QModule::progressTimeSeconds()
{
	return 180;
}

