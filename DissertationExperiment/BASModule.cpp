#include "stdafx.h"

#include "BASModule.h"
#include "Util.h"


// Constructor.
BASModule::BASModule()
{
	startInstructions = "Each item of this questionnaire is a statement that a person may either agree "
		"with or disagree with. For each item, indicate how much you agree or disagree "
		"with what the item says. "
		"Please be as "
		"accurate and honest as you can be. Respond to each item as if it were the "
		"only item. That is, don't worry about being \"consistent\" in your responses. "
		"Choose from the following four response options:";
	startInstructions = Util::getInstance().insertNewLines(startInstructions, 80);
	startInstructions = startInstructions + "\n\n"
		"1 = very true for me\n"
		"2 = somewhat true for me\n"
		"3 = somewhat false for me\n"
		"4 = very false for me";

	everyScreenInstructions = 
		"1 = very true for me\n"
		"2 = somewhat true for me\n"
		"3 = somewhat false for me\n"
		"4 = very false for me";

	questions.push_back("A person's family is the most important thing in life.");
	questions.push_back("Even if something bad is about to happen to me, I rarely \nexperience fear or nervousness.");
	questions.push_back("I go out of my way to get things I want.");
	questions.push_back("When I'm doing well at something I love to keep at it.");
	questions.push_back("I'm always willing to try something new if I think it will be fun.");
	questions.push_back("How I dress is important to me.");
	questions.push_back("When I get something I want, I feel excited and energized.");
	questions.push_back("Criticism or scolding hurts me quite a bit.");
	questions.push_back("When I want something I usually go all-out to get it.");
	questions.push_back("I will often do things for no other reason than that they might be fun.");
	questions.push_back("It's hard for me to find the time to do things such as get a haircut.");
	questions.push_back("If I see a chance to get something I want I move on it right away.");
	questions.push_back("I feel pretty worried or upset when I think or know somebody is angry at me.");
	questions.push_back("When I see an opportunity for something I like I get excited right away.");
	questions.push_back("I often act on the spur of the moment.");
	questions.push_back("If I think something unpleasant is going to happen I usually \nget pretty \"worked up.\"");
	questions.push_back("I often wonder why people act the way they do.");
	questions.push_back("When good things happen to me, it affects me strongly.");
	questions.push_back("I feel worried when I think I have done poorly at something important.");
	questions.push_back("I crave excitement and new sensations.");
	questions.push_back("When I go after something I use a \"no holds barred\" approach.");
	questions.push_back("I have very few fears compared to my friends.");
	questions.push_back("It would excite me to win a contest.");
	questions.push_back("I worry about making mistakes.");

	for (auto iter = questions.begin(); iter != questions.end(); ++iter) questionInfos.push_back("");

	// Allowable responses.
	allowableResponses = "1234";

	questionnaireName = "BIS BAS";

	state = START;
}


// Destructor.
BASModule::~BASModule()
{
}