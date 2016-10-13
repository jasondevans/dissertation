#pragma once
#include "stdafx.h"
#include "PrimingModule.h"

using namespace std;


class SimData
{

public:

	// Constructor.
	SimData();

	// Return a reference to our singleton.
	static SimData& getInstance()
	{
		static SimData instance;
		return instance;
	}

	// Run sim.
	void runSim();

	// Get an appropriate raw response.
	string getRawResponse(const PrimingModule& thisModule, int trialIndex, bool correct);

	// Destructor.
	~SimData();

};