// Includes.
#include "stdafx.h"
#include "SimData.h"
#include "Util.h"
#include "Module.h"
#include "PrimingModule.h"


// Constructor.
SimData::SimData()
{
}


// Destructor.
SimData::~SimData()
{
}


// Run sim.
void SimData::runSim()
{
	Util& util = Util::getInstance();
	vector<shared_ptr<PrimingModule>> modules;

	// Priming practice.
	modules.push_back(make_shared<PrimingModule>(false, true, true, 20, 0)); // 20
	modules.push_back(make_shared<PrimingModule>(true, true, true, 50, 1));  // 50
	modules.push_back(make_shared<PrimingModule>(true, true, true, 50, 2));  // 50
	modules.push_back(make_shared<PrimingModule>(true, false, false, 20, 3));  // 20 // For now, use real words (not practice)
	modules.push_back(make_shared<PrimingModule>(true, false, false, 50, 4));  // 50 // For now, use real words (not practice)
	modules.push_back(make_shared<PrimingModule>(true, false, false, 50, 4));  // 50 // For now, use real words (not practice)

	// First priming block.
	modules.push_back(make_shared<PrimingModule>(true, false, false, 50, 4));  // 50
	modules.push_back(make_shared<PrimingModule>(true, false, false, 50, 4));  // 50
	modules.push_back(make_shared<PrimingModule>(true, false, false, 50, 4));  // 50

	// Two more game-priming pairs.
	for (int i = 0; i < 2; i++)
	{
		// Priming.
		modules.push_back(make_shared<PrimingModule>(true, false, false, 50, 4));  // 50
		modules.push_back(make_shared<PrimingModule>(true, false, false, 50, 4));  // 50
		modules.push_back(make_shared<PrimingModule>(true, false, false, 50, 4));  // 50
	}

	boost::random::uniform_real_distribution<double> ureal_dist(0.0, 1.0);

	double nrProp = 0.03;
	double earlyProp = 0.07;
	double lateProp = 0.10;

	double practiceCorrectProp = 0.83;
	double congrCorrectProp = 0.87;
	double incongrCorrectProp = 0.87;

	for (auto &thisModule : modules)
	{
		thisModule->liveInitialize();
		for (int i = 0; i < thisModule->numTrials; i++)
		{
			thisModule->trialResponses.push_back(i + 1);
			bool nr = false;
			if (thisModule->withErrorFeedback)
			{
				thisModule->responseTypes.push_back("RESPONDED");
			}
			else
			{
				double typeRand = ureal_dist(util.randomGen);
				if (typeRand < nrProp) { thisModule->responseTypes.push_back("NR"); nr = true; }
				else if (typeRand < nrProp + earlyProp) thisModule->responseTypes.push_back("EARLY");
				else if (typeRand < nrProp + earlyProp + lateProp) thisModule->responseTypes.push_back("LATE");
				else thisModule->responseTypes.push_back("WINDOW");
			}
			if (nr)	thisModule->rawResponses.push_back("NR");
			else
			{
				double rawRand = ureal_dist(util.randomGen);
				if (thisModule->withErrorFeedback)
				{
					if (rawRand < practiceCorrectProp)	thisModule->rawResponses.push_back(getRawResponse(*thisModule, i, true));
					else thisModule->rawResponses.push_back(getRawResponse(*thisModule, i, false));
				}
				else if (thisModule->targetWordIsPleasant[i] == thisModule->primeWordIsSpatialApproach[i])
				{
					if (rawRand < congrCorrectProp)	thisModule->rawResponses.push_back(getRawResponse(*thisModule, i, true));
					else thisModule->rawResponses.push_back(getRawResponse(*thisModule, i, false));
				}
				else
				{
					if (rawRand < incongrCorrectProp)	thisModule->rawResponses.push_back(getRawResponse(*thisModule, i, true));
					else thisModule->rawResponses.push_back(getRawResponse(*thisModule, i, false));
				}
			}
			if (nr)	thisModule->standardResponses.push_back("NR");
			else
			{
				thisModule->standardResponses.push_back(
					thisModule->rawResponses[i].compare("f") == 0 ?
					(util.randomization_priming_keymap == 1 ? "pleasant" : "unpleasant") :
					(util.randomization_priming_keymap == 1 ? "unpleasant" : "pleasant"));
			}
		}
		thisModule->writeData();
	}

	util.finishAndCloseFiles();
}


// Get an appropriate raw response.
string SimData::getRawResponse(const PrimingModule& thisModule, int trialIndex, bool correct)
{
	if (thisModule.targetWordIsPleasant[trialIndex])
	{
		return
			Util::getInstance().randomization_priming_keymap == 1 ? 
			(correct ? "f" : "j") : 
			(correct ? "j" : "f");
	}
	else
	{
		return
			Util::getInstance().randomization_priming_keymap == 1 ?
			(correct ? "j" : "f") :
			(correct ? "f" : "j");
	}
}
