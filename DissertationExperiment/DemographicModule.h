#pragma once

#include "Module.h"

using namespace std;

class DemographicModule : public Module
{
	public:

	// Current state of the module.
	int state;

	// Keyboard states.
	bool anyKeyPressed;
	string key;
	bool shiftDown;

	// Prompt text.
	string instructions;
	string agePrompt;
	string genderPrompt;
	string raceEthnicityPrompt;
	string lhrhPrompt;

	// Response variables.
	string age;
	string gender;
	string raceEthnicity;
	string lhrh;

	// Allowable characters for each response.
	string ageAllowable;
	string genderAllowable;
	string raceEthnicityAllowable;
	string lhrhAllowable;

	// State constants.
	static const int STATE_AGE = 1;  // We're on the age field.
	static const int STATE_GENDER = 2;  // We're on the gender field.
	static const int STATE_RACEETHNICITY = 3;  // We're on the race/ethnicity field.
	static const int STATE_LHRH = 4;  // We're on the handedness field.
	static const int STATE_FINISHED = 5;  // We're done.

	// Constructor.
	DemographicModule();

	// Destructor.
	virtual ~DemographicModule();

	virtual int render_frame();

	// Handle keyboard input.
	virtual LRESULT handleInput(RAWINPUT InputData);

	virtual int progressTimeSeconds();
};