#include "stdafx.h"

#include "GRFModule.h"
#include "Util.h"


// Constructor.
GRFModule::GRFModule()
{
	startInstructions = "Using the scale below, please respond with the appropriate number for each item.";
	startInstructions = Util::getInstance().insertNewLines(startInstructions, 80);
	startInstructions = startInstructions + "\n\n"
		"Not at all true of me -- 1  2   3   4   5   6   7   8   9 -- Very true of me";

	everyScreenInstructions = "\n\nNot at all true of me -- 1  2   3   4   5   6   7   8   9 -- Very true of me";

	questions.push_back("In general, I am focused on preventing negative events in my life.");
	questions.push_back("I am anxious that I will fall short of my responsibilities and obligations.");
	questions.push_back("I frequently imagine how I will achieve my hopes and aspirations.");
	questions.push_back("I often think about the person I am afraid I might become in the future.");
	questions.push_back("I often think about the person I would ideally like to be in the future.");
	questions.push_back("I typically focus on the success I hope to achieve in the future.");
	questions.push_back("I often worry that I will fail to accomplish my goals.");
	questions.push_back("I often think about how I will achieve success.");
	questions.push_back("I often imagine myself experiencing bad things that I fear might happen to me.");
	questions.push_back("I frequently think about how I can prevent failures in my life.");
	questions.push_back("I am more oriented toward preventing losses than I am toward achieving gains.");
	questions.push_back("My major goal right now is to achieve my ambitions.");
	questions.push_back("My major goal right now is to avoid becoming a failure.");
	questions.push_back("I see myself as someone who is primarily striving to reach my \"ideal self\" -- \nto fulfill my hopes, wishes, and aspirations.");
	questions.push_back("I see myself as someone who is primarily striving to become the self I \"ought\" \nto be -- to fulfill my duties, responsibilities, and obligations.");
	questions.push_back("In general, I am focused on achieving positive outcomes in my life.");
	questions.push_back("I often imagine myself experiencing good things that I hope will happen to me.");
	questions.push_back("Overall, I am more oriented toward achieving success than preventing failure.");

	for (auto iter = questions.begin(); iter != questions.end(); ++iter) questionInfos.push_back("");

	allowableResponses = "123456789";

	questionnaireName = "General Regulatory Focus";

	state = START;
}


// Destructor.
GRFModule::~GRFModule()
{
}