#include "stdafx.h"

#include "DisplayUntilFinishedQubit.h"
#include "Util.h"
#include "Graphics.h"
#include "Color.h"

using namespace std;


// Constructor.
DisplayUntilFinishedQubit::DisplayUntilFinishedQubit(const string& displayText)
{
	finished = false;
	this->displayText = displayText;
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
DisplayUntilFinishedQubit::~DisplayUntilFinishedQubit()
{
}


int DisplayUntilFinishedQubit::render_frame()
{
	// Set up a new frame.
	Graphics::getInstance().setUpNewFrame();

	// Draw the text.
	Graphics::getInstance().drawText(displayText, position.left, position.top, Graphics::getInstance().standardFont, textColor);

	// Finish and present the frame.
	Graphics::getInstance().finishAndPresentFrame();

	// Check for key press and update state.
	if (finished)
	{
		return Util::QUBIT_STATE_FINISHED;
	}
	else
	{
		return Util::QUBIT_STATE_RUNNING;
	}
}


// Handle keyboard input.
LRESULT DisplayUntilFinishedQubit::handleInput(RAWINPUT InputData)
{
    return 0;
}



