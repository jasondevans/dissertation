#include "stdafx.h"

#include "PrimingModule.h"
#include "Util.h"
#include "Font.h"
#include "Graphics.h"
#include "Qubit.h"
#include "DisplayUntilKeypressQubit.h"
#include "DisplayForFramesQubit.h"
#include "DisplayForFramesInputColorChangeQubit.h"
#include "InputFeedbackQubit.h"


// Constructor.
PrimingModule::PrimingModule(bool withMasksAndPrimes, bool withErrorFeedback, bool isPractice, int numTrials, int instructionSetNum)
{
	// Whether to display masks and primes, or not display them.
	this->withMasksAndPrimes = withMasksAndPrimes;

	// Whether to use error feedback or response window.
	this->withErrorFeedback = withErrorFeedback;

	// Whether this is practice, which means use practice word lists instead of real word lists.
	this->isPractice = isPractice;

	// The number of trials.
	this->numTrials = numTrials;

	// Our instruction set number.
	this->instructionSetNum = instructionSetNum;

	// Vector of instructions.
	vector<string> instructions;

	// Determine key/mapping labels, based on randomization.
	string lLabel = "pleasant";
	string rLabel = "unpleasant";
	string lLabelCamel = "Pleasant";
	string rLabelCamel = "Unpleasant";
	if (Util::getInstance().randomization_priming_keymap == false)
	{
		lLabel = "unpleasant";
		rLabel = "pleasant";
		lLabelCamel = "Unpleasant";
		rLabelCamel = "Pleasant";
	}

	// Instructions 0.
	string instr0 = ""
		"" + Util::getInstance().insertNewLines(""
		"On the following screens, there will be a series of words presented, "
		"one at a time. Each word will be either pleasant (such as \"HAPPY\") "
		"or unpleasant (such as \"TRAGEDY\"). After each word is displayed, "
		"indicate whether that word is pleasant or unpleasant, in the following "
		"way.", 80) + "\n\n"
		"" + Util::getInstance().insertNewLines(""
		"Place your left index finger on the \"F\" key, and your right index "
		"finger on the \"J\" key. If the word is " + lLabel + ", press the \"F\" key "
		"with your left index finger. If the word is " + rLabel + ", press the \"J\" "
		"key with your right index finger. You will see reminders at the "
		"bottom of the screen if you forget.", 80) + "\n\n"
		"" + Util::getInstance().insertNewLines(""
		"If your response is correct, the next word will be presented. If "
		"your reponse is incorrect the word \"ERROR\" will flash briefly "
		"before the next word.  Just do your best -- it's okay to make mistakes.", 80);
	instructions.push_back(instr0);

	// Instructions 1.
	string instr1 = Util::getInstance().insertNewLines(""
		"On the following screens, you will again see a series of words and "
		"you are to indicate whether they are " + lLabel + " (\"F\" key) or "
		"" + rLabel + " (\"J\" key).", 80) + "\n\n" + Util::getInstance().insertNewLines(""
		"However, this time you will see a brief display of random letters "
		"(such as \"QRTSWCDLKHS\") before each word. Continue to respond "
		"to the word presented after the random letters.", 80) + "\n\n" + Util::getInstance().insertNewLines(""
		"Again, just do your best -- it's okay to "
		"make mistakes.", 80);
	instructions.push_back(instr1);

	// Instructions 2.
	string instr2 = Util::getInstance().insertNewLines(""
		"On the following screens, you will again see a series of words "
		"preceded by random letters.  Continue to indicate whether the words "
		"are " + lLabel + " (\"F\" key) or " + rLabel + " (\"J\" key).", 80);
	instructions.push_back(instr2);

	// Instructions 3.
	string instr3 = Util::getInstance().insertNewLines(""
		"This time, the words will be followed by an exclamation point (\"!\") for a "
		"brief period. Try to press your response key (" + lLabel + "/\"F\" or " + rLabel +"/\"J\") during the "
		"time the exclamation point is on the screen. Try not to respond either "
		"too early (before the exclamation point appears) or too late (after the "
		"exclamation point disappears. You can tell if you have done so in the "
		"following way:", 80) + "\n\n" + Util::getInstance().insertNewLines(""
		"If you respond too early, the exclamation point will not appear. So, if "
		"the exclamation point does not appear your response was too early.", 80) + "\n\n"
		"" + Util::getInstance().insertNewLines(""
		"Once you respond, the exclamation point will turn red. If the exclamation "
		"point turns red at any point, you have responded at the right time. If "
		"the exclamation point disappears before turning red, your response was too "
		"late. There will be no \"ERROR\" feedback.", 80) + "\n\n" + Util::getInstance().insertNewLines(""
		"In order to get a sense of the timing of the response window, simply watch "
		"the presentation of the first five words without responding, and begin "
		"responding on the sixth presentation (there will also be cues at the bottom "
		"of the screen to let you know when to begin responding).", 80) + "\n\n"
		"" + Util::getInstance().insertNewLines(""
		"Do your best to respond during the time the exclamation point "
		"is on the screen -- it's okay to make mistakes.", 80);
	instructions.push_back(instr3);

	// Instructions 4.
	string instr4 = Util::getInstance().insertNewLines(""
		"On the following screens, you will again see a series of words and you are to "
		"indicate if they are " + lLabel + " (\"F\" key) or " + rLabel + " (\"J\" key). "
		"As before, try to time your response so you respond while the exclamation point "
		"is on the screen.", 80) + "\n\n" + Util::getInstance().insertNewLines(""
		"If you respond too early, the exclamation point will not appear.", 80) + "\n\n"
		"" + Util::getInstance().insertNewLines(""
		"The exclamation point will turn red when you respond during the window.", 80) + "\n\n"
		"" + Util::getInstance().insertNewLines(""
		"If the exclamation point disappears before it turns red, you responded too late.", 80) + "\n\n"
		"" + Util::getInstance().insertNewLines(""
		"Again, simply watch the first five presentations to get a sense of timing, and "
		"begin responding on the sixth trial.", 80) + "\n\n"
		"" + Util::getInstance().insertNewLines(""
		"Do your best to respond during the time the exclamation point "
		"is on the screen -- it's okay to make mistakes.", 80);
	instructions.push_back(instr4);

	// Instructions qubit.
	string instrHeader = "========== WORD CATEGORIZATION ==========\n\n";
	string instrFooter = "\n\n\nPress any key to continue >>>>>";
	string instr = instrHeader + instructions[instructionSetNum] + instrFooter;
	pInstructionsQubit = new DisplayUntilKeypressQubit(instr, 50 /* 3 seconds */, true);

	// Add instructions state.
	State* instructionsState = new State(boost::bind(&PrimingModule::render_frame_instructions, boost::ref(*this)),
		boost::bind(&PrimingModule::handleInputInstructions, boost::ref(*this), _1));
	instructionsState->initializeFn = boost::bind(&PrimingModule::liveInitialize, boost::ref(*this));
	stateManager.states.push_back(instructionsState);

	// Initialize post-instructions pause qubit pointer.
	pPostInstructionsPauseQubit = NULL;

	// Summary qubit.
	pSummaryQubit = new DisplayUntilKeypressQubit("");
}


void PrimingModule::liveInitialize()
{
	// Bottom text.
	string bottomText = "(Pleasant / F)                                                    (J / Unpleasant)";
	string bottomReplaceText = "(Pleasant / F)                           *                      (J / Unpleasant)";
	if (Util::getInstance().randomization_priming_keymap == false)
	{
		bottomText = "(Unpleasant / F)                                                    (J / Pleasant)";
		bottomReplaceText = "(Unpleasant / F)                         *                        (J / Pleasant)";
	}

	// Add brief blank screen for subjects to prepare.
	pPostInstructionsPauseQubit = new DisplayForFramesQubit("", bottomText, 60 * 1, Graphics::getInstance().wordFont);
	State* pipState = new State(*pPostInstructionsPauseQubit);
	if (withErrorFeedback)	stateManager.states.push_back(pipState);

	// Initialize summary text.
	summaryText = "";

	// Initializations.
	currentTrialIndex = 0;
	currentTrialQubitIndex = 0;

	// Create lead-in trials, if appropriate.
	if (!withErrorFeedback)
	{
		// Create a list of lead-in prime words.
		leadInPrimeWords = Util::getInstance().randomElements(Util::getInstance().gamePracticeWords, 5);

		// Create a list of lead-in target words.
		leadInTargetWords = Util::getInstance().randomList(Util::getInstance().pleasantWords, Util::getInstance().unpleasantWords, 5);
	}

	// Figure out which word lists to use.
	vector<string> primeWordsA;
	vector<string> primeWordsB;
	if (Util::getInstance().replicateDG)
	{
		primeWordsA = Util::getInstance().randomization_game_wordset ? Util::getInstance().pleasantWords
			: Util::getInstance().unpleasantWords;
		primeWordsB = Util::getInstance().randomization_game_wordset ? Util::getInstance().unpleasantWords
			: Util::getInstance().pleasantWords;
	}
	else
	{
		primeWordsA = isPractice ? Util::getInstance().gamePracticeWords : Util::getInstance().firstGameWords;
		primeWordsB = isPractice ? Util::getInstance().gamePracticeWords : Util::getInstance().secondGameWords;
	}
            
    // Create a randomized list of prime words.
	primeWords = Util::getInstance().randomList(primeWordsA, primeWordsB, numTrials);

    // Create a randomized list of target words.
	targetWords = Util::getInstance().randomList(Util::getInstance().pleasantWords, Util::getInstance().unpleasantWords, numTrials);

	// Determine whether each target word is pleasant or unpleasant (for determining correct responses).
	for (auto iter = targetWords.begin(); iter != targetWords.end(); ++iter)
	{
		targetWordIsPleasant.push_back(
			std::find(Util::getInstance().pleasantWords.begin(), Util::getInstance().pleasantWords.end(), (*iter)) 
			!= Util::getInstance().pleasantWords.end() ? true : false);
	}

	// Determine whether the spatial action associated with each prime word is approach / pleasant (vs. avoid / unpleasant).
	if (withMasksAndPrimes) for (auto iter = primeWords.begin(); iter != primeWords.end(); ++iter)
	{
		if (Util::getInstance().replicateDG)
		{
			primeWordIsSpatialApproach.push_back(
				std::find(Util::getInstance().pleasantWords.begin(), Util::getInstance().pleasantWords.end(), (*iter)) 
				!= Util::getInstance().pleasantWords.end() ? true : false);
			primeWordIsConceptualApproach.push_back(
				std::find(Util::getInstance().pleasantWords.begin(), Util::getInstance().pleasantWords.end(), (*iter)) 
				!= Util::getInstance().pleasantWords.end() ? true : false);
		}
		else
		{
			primeWordIsSpatialApproach.push_back(
				std::find(Util::getInstance().firstGameWords.begin(), Util::getInstance().firstGameWords.end(), (*iter))
				!= Util::getInstance().firstGameWords.end() ? true : false);
			primeWordIsConceptualApproach.push_back(
				std::find(Util::getInstance().firstGameWords.begin(), Util::getInstance().firstGameWords.end(), (*iter))
				!= Util::getInstance().firstGameWords.end() ? 
				(Util::getInstance().randomization_game_instructions ? true : false) :
				(Util::getInstance().randomization_game_instructions ? false : true));
		}
	}

	// Create lead-in trials, with appropriate stimulus presentations for each trial.
	// Each trial is a vector of pointers to Qubit's.
	// (Will only be called if appropriate, i.e. leadInPrimeWords.size() > 0.)
	for (unsigned int i = 0; i < leadInPrimeWords.size(); i++)
	{
		Font* font = Graphics::getInstance().wordFont;

		// Determine the bottom text.
		string thisBottomText = bottomReplaceText;
		std::replace(thisBottomText.begin(), thisBottomText.end(), '*', (char) ('5' - (char) i));

		vector<Qubit*> thisTrial;

		if (withMasksAndPrimes)
		{
			string randomStr1 = Util::getInstance().CreateRandomConsonantString(Util::getInstance().maskLength, "");
			string randomStr2 = Util::getInstance().CreateRandomConsonantString(Util::getInstance().maskLength, randomStr1);
			thisTrial.push_back(new DisplayForFramesQubit(randomStr1, thisBottomText, 9, font));
			thisTrial.push_back(new DisplayForFramesQubit(leadInPrimeWords[i], thisBottomText, 2, font));
			thisTrial.push_back(new DisplayForFramesQubit(randomStr2, thisBottomText, 1, font));
		}

		thisTrial.push_back(new DisplayForFramesQubit(leadInTargetWords[i], thisBottomText, 11, font));

		if (withErrorFeedback)  // display error feedback
		{
			thisTrial.push_back(new InputFeedbackQubit(thisBottomText));
			thisTrial.push_back(new DisplayForFramesQubit("", thisBottomText, 20, font));
		}
		else  // use a response window
		{
			thisTrial.push_back(new DisplayForFramesQubit("", thisBottomText, Util::getInstance().framesBeforeResponseWindow, font));
			thisTrial.push_back(new DisplayForFramesInputColorChangeQubit("!", thisBottomText, 8, font));
			thisTrial.push_back(new DisplayForFramesQubit("", thisBottomText, 36, font));
		}

		leadInTrials.push_back(thisTrial);
	}

	// Create trials, with appropriate stimulus presentations for each trial.
	// Each trial is a vector of pointers to Qubit's.
	for (unsigned int i = 0; i < primeWords.size(); i++)
	{
		Font* font = Graphics::getInstance().wordFont;

		vector<Qubit*> thisTrial;

		if (withMasksAndPrimes)
		{
			string randomStr1 = Util::getInstance().CreateRandomConsonantString(Util::getInstance().maskLength, "");
			string randomStr2 = Util::getInstance().CreateRandomConsonantString(Util::getInstance().maskLength, randomStr1);
			thisTrial.push_back(new DisplayForFramesQubit(randomStr1, bottomText, 9, font));
			thisTrial.push_back(new DisplayForFramesQubit(primeWords[i], bottomText, 2, font));
			thisTrial.push_back(new DisplayForFramesQubit(randomStr2, bottomText, 1, font));
		}

		thisTrial.push_back(new DisplayForFramesQubit(targetWords[i], bottomText, 11, font));

		if (withErrorFeedback)  // display error feedback
		{
			thisTrial.push_back(new InputFeedbackQubit(bottomText));
			thisTrial.push_back(new DisplayForFramesQubit("", bottomText, 20, font));
		}
		else  // use a response window
		{
			thisTrial.push_back(new DisplayForFramesQubit("", bottomText, Util::getInstance().framesBeforeResponseWindow, font));
			thisTrial.push_back(new DisplayForFramesInputColorChangeQubit("!", bottomText, 8, font));
			thisTrial.push_back(new DisplayForFramesQubit("", bottomText, 36, font));
		}

		trials.push_back(thisTrial);
	}

	// Add lead-in state, if appropriate.
	State* leadInState = new State();
	if (!withErrorFeedback)
	{
		leadInState->executeStepFn = boost::bind(&PrimingModule::render_frame_leadin, boost::ref(*this));
		leadInState->handleInputFn = boost::bind(&PrimingModule::handleInputGeneral, boost::ref(*this), _1);
		stateManager.states.push_back(leadInState);
	}

	// Add main trials.
	State* inProgressState = new State(boost::bind(&PrimingModule::render_frame_inprogress, boost::ref(*this)),
		boost::bind(&PrimingModule::handleInputGeneral, boost::ref(*this), _1));
	stateManager.states.push_back(inProgressState);

	// Add the summary.
	State* summaryState = new State(boost::bind(&PrimingModule::render_frame_summary, boost::ref(*this)),
		boost::bind(&PrimingModule::handleInputSummary, boost::ref(*this), _1));
	stateManager.states.push_back(summaryState);

	// Initialize priming timing variables.
	Util::getInstance().primingTimingEnabled = true;
	Util::getInstance().primingTimingCanRespond = false;
	Util::getInstance().primingTimingHasResponded = false;
	Util::getInstance().primingTimingCountdownFrames = 0;
}


// Destructor.
PrimingModule::~PrimingModule()
{
	delete pInstructionsQubit;

	delete pSummaryQubit;

	if (pPostInstructionsPauseQubit) delete pPostInstructionsPauseQubit;

	for (auto iter = trials.begin(); iter != trials.end(); ++iter)
	{
		for (auto jter = (*iter).begin(); jter != (*iter).end(); ++jter)
		{
			delete (*jter);
		}
	}

	for (auto iter = stateManager.states.begin(); iter != stateManager.states.end(); ++iter)
	{
		delete (*iter);
	}
}


int PrimingModule::render_frame()
{
	int retVal = stateManager.executeStep();
	if (retVal == StateManager::STATE_MANAGER_FINISHED) return Util::MODULE_STATE_FINISHED;
	else return Util::MODULE_STATE_RUNNING;
}


int PrimingModule::render_frame_instructions()
{
	int qubitReturn = pInstructionsQubit->render_frame();
	if (qubitReturn == Util::QUBIT_STATE_FINISHED)
	{
		resetTrialData();
		return State::STATE_TRANSITION_NEXT;
	}
	return State::STATE_TRANSITION_KEEP_CURRENT;
}


int PrimingModule::render_frame_summary()
{
	// Compute statistics, build the summary string if we haven't already.
	if (summaryText.compare("") == 0)
	{
		// Determine the summary data to display.
		int numTrials = rawResponses.size();
		int numWithinWindow = 0;
		int numCorrect = 0;
		for (unsigned int i = 0; i < rawResponses.size(); i++)
		{
			if (responseTypes[i].compare("WINDOW") == 0)
			{
				numWithinWindow++;
				if (standardResponses[i].compare(targetWordIsPleasant[i] ? "pleasant" : "unpleasant") == 0)
					numCorrect++;
			}
			if (responseTypes[i].compare("RESPONDED") == 0)
			{
				if (standardResponses[i].compare(targetWordIsPleasant[i] ? "pleasant" : "unpleasant") == 0)
					numCorrect++;
			}
		}
		float percentWithinWindow = (float) numWithinWindow / (float) numTrials * 100.0f;
		float percentCorrect = (float) numCorrect / (float) numTrials * 100.0f;

		// Adjust the "frames before response window", depending on performance.
		if (!withErrorFeedback) // i.e., we used a response window.
		{
			if (percentCorrect > 80.0f) Util::getInstance().framesBeforeResponseWindow -= 2;
			if (percentCorrect < 60.0f) Util::getInstance().framesBeforeResponseWindow += 2; // 55 in the articles
		}

		// Build the summary string.
		std::stringstream sstm;
		if (!withErrorFeedback)
		{
			sstm << "Percent within the response window:  " << std::setprecision(0) << percentWithinWindow << "%\n\n";
		}
		sstm << "Percent correct:  " << std::setprecision(0) << std::setprecision(0) << percentCorrect << "%\n\n\n\n";
		sstm << "(Press any key to continue)";

		// Save it to our summary text variable.
		summaryText = sstm.str();

		// Live initialize the summary qubit.
		pSummaryQubit->data["text"] = summaryText;
		pSummaryQubit->liveInitialize();
	}

	// Call the qubit.
	int qubitReturn = pSummaryQubit->render_frame();
	if (qubitReturn == Util::QUBIT_STATE_FINISHED) return State::STATE_TRANSITION_NEXT;
	else return State::STATE_TRANSITION_KEEP_CURRENT;
}


int PrimingModule::render_frame_leadin()
{
	// Show lead-in trials.
	vector<Qubit*> currentTrial = leadInTrials[currentTrialIndex];
	Qubit* pCurrentQubit = currentTrial[currentTrialQubitIndex];
	int qubitReturn = pCurrentQubit->render_frame();

	// If this qubit is finished, move on to the next one.
	if (qubitReturn == Util::QUBIT_STATE_FINISHED)
	{
		if (currentTrialIndex == leadInTrials.size() - 1 && currentTrialQubitIndex == currentTrial.size() - 1)
		{
			// Set up real trials.
			currentTrialIndex = 0;
			currentTrialQubitIndex = 0;
			resetTrialData();
			return State::STATE_TRANSITION_NEXT;
		}
		else if (currentTrialQubitIndex == currentTrial.size() - 1)
		{
			// Move to the next trial.
			currentTrialIndex++;
			currentTrialQubitIndex = 0;
			resetTrialData();
			return State::STATE_TRANSITION_KEEP_CURRENT;
		}
		else
		{
			// Move to the next qubit in this trial.
			trialData = pCurrentQubit->data;
			currentTrialQubitIndex++;
			currentTrial[currentTrialQubitIndex]->data = trialData;
			currentTrial[currentTrialQubitIndex]->liveInitialize();
			return State::STATE_TRANSITION_KEEP_CURRENT;
		}
	}

	// Return that we're still running.
	return State::STATE_TRANSITION_KEEP_CURRENT; 
}


int PrimingModule::render_frame_inprogress()
{
	// Carry out the main priming task.
	vector<Qubit*> currentTrial = trials[currentTrialIndex];
	Qubit* pCurrentQubit = currentTrial[currentTrialQubitIndex];
	int qubitReturn = pCurrentQubit->render_frame();

	// If this qubit is finished, move on to the next one.
	if (qubitReturn == Util::QUBIT_STATE_FINISHED)
	{
		if (currentTrialQubitIndex == currentTrial.size() - 1) // Finished this trial.
		{
			// Save user response.
			rawResponses.push_back(pCurrentQubit->data["responseKey"]);
			standardResponses.push_back(pCurrentQubit->data["response"]);
			responseTypes.push_back(pCurrentQubit->data["responseType"]);
			trialResponses.push_back(currentTrialIndex + 1);

			// Reset priming timing variables.
			Util::getInstance().primingTimingCanRespond = false;
			Util::getInstance().primingTimingHasResponded = false;
			Util::getInstance().primingTimingCountdownFrames = 0;
		}

		if (currentTrialIndex == trials.size() - 1 && currentTrialQubitIndex == currentTrial.size() - 1)
		{
			// Write our data.
			writeData();

			// Turn off priming timing.
			Util::getInstance().primingTimingEnabled = false;

			// Return that main trials are finished.
			return State::STATE_TRANSITION_NEXT;
		}
		else if (currentTrialQubitIndex == currentTrial.size() - 1)
		{
			// Move to the next trial.
			currentTrialIndex++;
			currentTrialQubitIndex = 0;
			resetTrialData();
			return State::STATE_TRANSITION_KEEP_CURRENT;
		}
		else
		{
			// Move to the next qubit in this trial.
			trialData = pCurrentQubit->data;
			currentTrialQubitIndex++;
			currentTrial[currentTrialQubitIndex]->data = trialData;
			currentTrial[currentTrialQubitIndex]->liveInitialize();
			return State::STATE_TRANSITION_KEEP_CURRENT;
		}
	}

	// Return that we're still running.
	return State::STATE_TRANSITION_KEEP_CURRENT; 
}


LRESULT PrimingModule::handleInput(RAWINPUT InputData)
{
	return stateManager.handleInput(InputData);
}


// Handle keyboard input.
LRESULT PrimingModule::handleInputGeneral(RAWINPUT InputData)
{
	vector<Qubit*> currentTrial = trials[currentTrialIndex];
	Qubit* pCurrentQubit = currentTrial[currentTrialQubitIndex];
	return pCurrentQubit->handleInput(InputData);
}


LRESULT PrimingModule::handleInputInstructions(RAWINPUT InputData)
{
	return pInstructionsQubit->handleInput(InputData);
}


LRESULT PrimingModule::handleInputSummary(RAWINPUT InputData)
{
	return pSummaryQubit->handleInput(InputData);
}


void PrimingModule::writeData()
{
	Util::getInstance().outputSS << "<priming>" << endl;

	Util::getInstance().outputSS << "<info>" << endl;
	Util::getInstance().outputSS << "<with-masks-and-primes>" << (withMasksAndPrimes ? "true" : "false") << "</with-masks-and-primes>" << endl;
	Util::getInstance().outputSS << "<with-error-feedback>" << (withErrorFeedback ? "true" : "false") << "</with-error-feedback>" << endl;
	Util::getInstance().outputSS << "<with-practice-primes>" << (isPractice ? "true" : "false") << "</with-practice-primes>" << endl;
	Util::getInstance().outputSS << "<num-trials>" << numTrials << "</num-trials>" << endl;
	Util::getInstance().outputSS << "<instruction-set-num>" << instructionSetNum << "</instruction-set-num>" << endl;
	Util::getInstance().outputSS << "</info>" << endl;

	for (unsigned int i = 0; i < rawResponses.size(); i++)
	{
		Util::getInstance().outputSS << "<trial>" << endl;
		Util::getInstance().outputSS << "<trial-number>" << trialResponses[i] << "</trial-number>" << endl;

		string thisPrimeWord;
		string thisTargetWord;
		thisPrimeWord = primeWords[trialResponses[i] - 1];
		thisTargetWord = targetWords[trialResponses[i] - 1];
		if (withMasksAndPrimes)
		{
			Util::getInstance().outputSS << "<prime-word>" << thisPrimeWord << "</prime-word>" << endl;
			if (!isPractice)
			{
				Util::getInstance().outputSS << "<prime-spatial-action>" << (primeWordIsSpatialApproach[i] ? "approach" : "avoid") << "</prime-spatial-action>" << endl;
				Util::getInstance().outputSS << "<prime-conceptual-action>" << (primeWordIsConceptualApproach[i] ? "approach" : "avoid") << "</prime-conceptual-action>" << endl;
			}
		}
		Util::getInstance().outputSS << "<target-word>" << thisTargetWord	<< "</target-word>" << endl;
		Util::getInstance().outputSS << "<target-valence>" << (targetWordIsPleasant[i] ? "pleasant" : "unpleasant") << "</target-valence>" << endl;
		Util::getInstance().outputSS << "<response-type>" << responseTypes[i] << "</response-type>" << endl;
		Util::getInstance().outputSS << "<raw-response>" << rawResponses[i] << "</raw-response>" << endl;
		Util::getInstance().outputSS << "<standard-response>" << standardResponses[i] << "</standard-response>" << endl;
		Util::getInstance().outputSS << "</trial>" << endl;
	}
	Util::getInstance().outputSS << "</priming>" << endl;
}


void PrimingModule::resetTrialData()
{
	trialData.clear();
	trialData["hasResponded"] = "false";
	trialData["primeWord"] = primeWords[currentTrialIndex];
	trialData["targetWord"] = targetWords[currentTrialIndex];
	trialData["targetIsPleasant"] = targetWordIsPleasant[currentTrialIndex] ? "true" : "false";
	trialData["responseKey"] = "NR";
	trialData["response"] = "NR";
	trialData["responseType"] = "NR";
	trialData["exclamationPresented"] = "false";
	(trials[currentTrialIndex])[currentTrialQubitIndex]->data = trialData;
	(trials[currentTrialIndex])[currentTrialQubitIndex]->liveInitialize();
}


int PrimingModule::progressTimeSeconds()
{
	return 360;
}

