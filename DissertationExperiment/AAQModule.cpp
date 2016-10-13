#include "stdafx.h"

#include "AAQModule.h"
#include "Util.h"


// Constructor.
AAQModule::AAQModule()
{
	//startInstructions = "Test";
	startInstructions = "On the following screens you will be shown a series of statements. "
		"Please rate how true each statement "
		"is for you by pressing the appropriate number key on the keyboard. Use the scale below to make your choice. (The "
		"scale will also be shown on the following screens to help you remember).";
	startInstructions = Util::getInstance().insertNewLines(startInstructions, 80);
	startInstructions = startInstructions + "\n\n"
		"1 = never true\n"
		"2 = very seldom true\n"
		"3 = seldom true\n"
		"4 = sometimes true\n"
		"5 = frequently true\n"
		"6 = almost always true\n"
		"7 = always true\n";

	everyScreenInstructions = ""
		"1 = never true\n"
		"2 = very seldom true\n"
		"3 = seldom true\n"
		"4 = sometimes true\n"
		"5 = frequently true\n"
		"6 = almost always true\n"
		"7 = always true\n";
	
	questions.push_back("My painful experiences and memories make it difficult for me \nto live a life that I would value.");
	questions.push_back("I'm afraid of my feelings.");
	questions.push_back("I worry about not being able to control my worries and feelings.");
	questions.push_back("My painful memories prevent me from having a fulfilling life.");
	questions.push_back("Emotions cause problems in my life.");
	questions.push_back("It seems like most people are handling their lives better than I am.");
	questions.push_back("Worries get in the way of my success.");

	for (auto iter = questions.begin(); iter != questions.end(); ++iter) questionInfos.push_back("");

	allowableResponses = "1234567";

	questionnaireName = "Acceptance and Action Questionnaire";

	state = START;
}


// Destructor.
AAQModule::~AAQModule()
{
}