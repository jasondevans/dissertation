#include "stdafx.h"

#include "EAMModule.h"
#include "Util.h"


// Constructor.
EAMModule::EAMModule()
{
	startInstructions = "Rate the pleasantness of the following words from the game on the following scale:";
	startInstructions = Util::getInstance().insertNewLines(startInstructions, 80);
	startInstructions = startInstructions + "\n\n"
		"1 Very Unpleasant  |  2 Unpleasant  |  3 Neutral  |  4 Pleasant  |  5 Very Pleasant";

	everyScreenInstructions = "\n\n1 Very Unpleasant  |  2 Unpleasant  |  3 Neutral  |  4 Pleasant  |  5 Very Pleasant";

	// Questions.
	vector<string> targetWords;
	for (auto iter = Util::getInstance().firstGameWords.begin(); iter != Util::getInstance().firstGameWords.end(); ++iter) targetWords.push_back(*iter);
	for (auto iter = Util::getInstance().secondGameWords.begin(); iter != Util::getInstance().secondGameWords.end(); ++iter) targetWords.push_back(*iter);
	Util::getInstance().randomSort(targetWords);
	for (auto iter = targetWords.begin(); iter != targetWords.end(); ++iter) questionInfos.push_back(*iter);
	for (auto iter = targetWords.begin(); iter != targetWords.end(); ++iter) questions.push_back(*iter);

	// Allowable responses.
	allowableResponses = "12345";

	questionnaireName = "Explicit Attitude Measurement";

	state = START;
}


// Destructor.
EAMModule::~EAMModule()
{
}