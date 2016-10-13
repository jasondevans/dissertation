#pragma once

#include "Module.h"
#include "Qubit.h"
#include "StateManager.h"


using namespace std;


class PrimingModule : public Module
{

public:

	// Instructions qubit.
	Qubit* pInstructionsQubit;

	// Post-instructions pause qubit.
	Qubit* pPostInstructionsPauseQubit;

	// Summary qubit.
	Qubit* pSummaryQubit;

	// Our state manager.
	StateManager stateManager;

	// Whether to display masks and primes.
	bool withMasksAndPrimes;

	// Whether to display error feedback (if true), or a response window (if false).
	bool withErrorFeedback;

	// Whether this is practice (meaning, use practice word lists instead of real word lists).
	bool isPractice;

	// The number of trials.
	int numTrials;

	// The number of the instruction set to use.
	int instructionSetNum;

    // List of trials.
	vector<vector<Qubit*>> trials;

	// List of lead-in trials.
	vector<vector<Qubit*>> leadInTrials;

	// Our lead-in words;
	vector<string> leadInPrimeWords;

	// Our lead-in target words;
	vector<string> leadInTargetWords;

	// Our prime words.
	vector<string> primeWords;

	// Our target words.
	vector<string> targetWords;

	// Whether each target word is pleasant (vs. unpleasant).
	vector<bool> targetWordIsPleasant;

	// Whether the spatial action associated with each priming word is approach (vs. avoid).
	vector<bool> primeWordIsSpatialApproach;

	// Whether the conceptual action associated with each priming word is approach (vs. avoid).
	vector<bool> primeWordIsConceptualApproach;

	// The current trial index.
	int currentTrialIndex;

	// The current qubit index, within the current trial.
	int currentTrialQubitIndex;

	// Data maintained across qubits over a single trial.
	map<string, string> trialData;

	// Raw key responses.
	vector<string> rawResponses;

	// Standard (A/B) key responses.
	vector<string> standardResponses;

	// Response types -- NR, WINDOW, EARLY, LATE.
	vector<string> responseTypes;

	// The trial numbers for each response.
	vector<int> trialResponses;

	// The summary text to display (will be empty until built at the end of the module).
	string summaryText;

	// Constructor.
	PrimingModule(bool withMasksAndPrimes, bool withErrorFeedback, bool isPractice, int numTrials, int instructionSetNum);

	// Live initialization.
	void liveInitialize();

	// Destructor.
	~PrimingModule();

	virtual int render_frame();

	virtual int render_frame_instructions();

	virtual int render_frame_summary();

	virtual int render_frame_leadin();

	virtual int render_frame_inprogress();

	// Handle keyboard input.
	virtual LRESULT handleInput(RAWINPUT InputData);

	virtual LRESULT handleInputInstructions(RAWINPUT InputData);

	virtual LRESULT handleInputSummary(RAWINPUT InputData);

	virtual LRESULT handleInputGeneral(RAWINPUT InputData);

	virtual int progressTimeSeconds();

	void writeData();

private:

	void resetTrialData();

};
