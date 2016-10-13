#include "stdafx.h"

#include "DisplayUntilKeypressQubit.h"
#include "Util.h"
#include "Graphics.h"
#include "Color.h"

using namespace std;


// Constructor.
DisplayUntilKeypressQubit::DisplayUntilKeypressQubit(const string& displayText)
{
	commonInitialization(displayText, -1, false);
}


// Constructor.
DisplayUntilKeypressQubit::DisplayUntilKeypressQubit(const string& displayText, bool showProgress)
{
	commonInitialization(displayText, -1, showProgress);
}


// Constructor.
DisplayUntilKeypressQubit::DisplayUntilKeypressQubit(const string& displayText, int minFrames)
{
	commonInitialization(displayText, minFrames, false);
}


// Constructor.
DisplayUntilKeypressQubit::DisplayUntilKeypressQubit(const string& displayText, int minFrames, bool showProgress)
{
	commonInitialization(displayText, minFrames, showProgress);
}


// Do common initialization.
void DisplayUntilKeypressQubit::commonInitialization(const string& displayText, int minFrames, bool showProgress)
{
	anyKeyPressed = false;
	this->displayText = displayText;
	this->minFrames = minFrames;
	this->numFramesDisplayed = 0;
	this->showProgress = showProgress;
	this->textColor = Color(255, 255, 255, 255); // White

    // Declare positioning variables.
    int wordWidth;
    int wordHeight;
	int positionX;
	int positionY;

	// Determine dimensions.
	RECT textDimensions = Graphics::getInstance().measureString(Graphics::getInstance().standardFont, displayText);
	wordWidth = textDimensions.right - textDimensions.left;
	wordHeight = textDimensions.bottom - textDimensions.top;
	positionX = (Graphics::getInstance().displayWidth - wordWidth) / 2;
	positionY = (Graphics::getInstance().displayHeight - wordHeight) / 2;

	// Set the position.
	SetRect(&position, positionX, positionY, Graphics::getInstance().displayWidth, Graphics::getInstance().displayHeight); 
}



// Destructor.
DisplayUntilKeypressQubit::~DisplayUntilKeypressQubit()
{
}


// Initialize just before first execution.
void DisplayUntilKeypressQubit::liveInitialize()
{
	displayText = data["text"];

	// Declare positioning variables.
    int wordWidth;
    int wordHeight;
	int positionX;
	int positionY;

	// Determine dimensions.
	RECT textDimensions = Graphics::getInstance().measureString(Graphics::getInstance().standardFont, displayText);
	wordWidth = textDimensions.right - textDimensions.left;
	wordHeight = textDimensions.bottom - textDimensions.top;
	positionX = (Graphics::getInstance().displayWidth - wordWidth) / 2;
	positionY = (Graphics::getInstance().displayHeight - wordHeight) / 2;

	// Set the position.
	SetRect(&position, positionX, positionY, Graphics::getInstance().displayWidth, Graphics::getInstance().displayHeight); 
}


int DisplayUntilKeypressQubit::render_frame()
{
	// Set up a new frame.
	Graphics::getInstance().setUpNewFrame();

	// Draw the text.
	Graphics::getInstance().drawText(displayText, position.left, position.top, Graphics::getInstance().standardFont, textColor);

	// Potentially display a progress indicator.
	if (showProgress) Graphics::getInstance().drawProgressText();

	// Finish and present the frame.
	Graphics::getInstance().finishAndPresentFrame();

	// Update the number of frames displayed.
	numFramesDisplayed++;

	// Check for key press and update state.
	if (anyKeyPressed)
	{
		if (minFrames < 0 || numFramesDisplayed >= minFrames)
		{
			return Util::QUBIT_STATE_FINISHED;
		}
		else
		{
			anyKeyPressed = false;
			return Util::QUBIT_STATE_RUNNING;
		}
	}
	else
	{
		return Util::QUBIT_STATE_RUNNING;
	}
}


// Handle keyboard input.
LRESULT DisplayUntilKeypressQubit::handleInput(RAWINPUT InputData)
{
	if (InputData.data.keyboard.Message == WM_KEYDOWN) anyKeyPressed = true;
    return 0;
}



