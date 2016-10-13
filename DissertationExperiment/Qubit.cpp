#include "stdafx.h"

#include "Qubit.h"
#include "Util.h"


Qubit::Qubit()
{
}


Qubit::~Qubit()
{
}


void Qubit::liveInitialize()
{
}


int Qubit::render_frame()
{
	return Util::QUBIT_STATE_FINISHED;
}


LRESULT Qubit::handleInput(RAWINPUT InputData)
{
	return 0;
}


void Qubit::cleanup()
{
}