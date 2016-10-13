// Includes.
#include "stdafx.h"
#include "Main.h"
#include "Util.h"
#include "Graphics.h"
#include "ExperimentMain.h"
#include "SimData.h"


// Program entry point.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// Initialize the experiment.
	ExperimentMain& experimentMain = ExperimentMain::getInstance();
	bool succeeded = experimentMain.initialize(hInstance, nCmdShow);
	if (!succeeded) return -1;

	bool runSim = false;

	if (runSim)
	{
		/*SimData& simData = SimData::getInstance();
		simData.runSim();*/
	}
	else
	{
		// Main loop.
		MSG msg;
		while (true)
		{
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			if (msg.message == WM_QUIT) break;
			if (experimentMain.renderFrame() == Util::EXPERIMENT_STATE_FINISHED) break;
		}
	}

	// Clean up the experiment.
	experimentMain.cleanUp();

    return 0;
}




