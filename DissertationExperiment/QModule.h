#pragma once

#include "Module.h"

using namespace std;

class QModule : public Module
{

public:

	// Whether this QModule has been initialized.
	bool initialized;

	// Start instructions.
	string startInstructions;

	// Every screen instructions;
	string everyScreenInstructions;

	// Question items.
	vector<string> questions;

	// Question info (optional).
	vector<string> questionInfos;

	// Responses to each question.
	vector<string> responses;

	// String containing characters that are allowable responses.
	string allowableResponses;

	// Current state of the module.
	int state;

	// Keyboard states.
	bool anyKeyPressed;
	string key;

	// This questionnaire's name.
	string questionnaireName;

	// State constants.
	static const int START = 0;  // At the beginning.
	static const int QUESTIONS = 1;  // Doing questions.
	static const int FINISHED = 2;  // We're done.

	// Which question number to display.
	int currentQ;

	// Constructor.
	QModule();

	// Constructor.
	QModule(string startInstructions, string everyScreenInstructions, vector<string> questions, 
		vector<string> questionInfos, string allowableResponses, string questionnaireName);

	// Destructor.
	virtual ~QModule();

	virtual int render_frame();

	// Handle keyboard input.
	virtual LRESULT handleInput(RAWINPUT InputData);

	virtual int progressTimeSeconds();

};