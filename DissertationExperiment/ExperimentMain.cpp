// Includes.
#include "stdafx.h"
#include "ExperimentMain.h"
#include "GameModule.h"
#include "PrimingModule.h"
#include "QModule.h"
#include "Util.h"
#include "Graphics.h"
#include "AAQModule.h"
#include "BASModule.h"
#include "GRFModule.h"
#include "EAMModule.h"
#include "ConsentModule.h"
#include "UploadModule.h"
#include "DemographicModule.h"
#include "DebriefingModule.h"
#include "ExecuteCodeModule.h"


// Constructor.
ExperimentMain::ExperimentMain()
{
}


// Initialize the experiment.
// Returns true if succeeded, false if failed.
bool ExperimentMain::initialize(HINSTANCE hInstance, int nCmdShow)
{
	// Test our ftp connectivity (and give user the chance to allow through firewall).
	if (Util::getInstance().uploadResults)
	{
		int uploadTestSucceeded = Util::getInstance().testFtpConnection();
		
		// Let the user know the results of the server connectivity test.
		if (!uploadTestSucceeded) 
		{
			MessageBox(NULL, L"The program could not connect to the server.  Please let Jason"
			L" know at email@domain.com.  Thank you!", L"Unable to connect",
			MB_ICONEXCLAMATION | MB_OK);
			return false;
		}
		else MessageBox(NULL, L"Test succeeded!  Click \"OK\" to begin the experiment.",
			L"Test Succeeded", MB_ICONINFORMATION | MB_OK);
	}

	// Attempt to set the display refresh rate.
	int refreshRateResult = Graphics::getInstance().setDisplayRefreshRate(60);
	if (refreshRateResult != 0)
	{
		int msgBoxID = MessageBox(NULL, 
			L"Jason's Dissertation Experiment was unable to start successfully\n"
			L"because it couldn't set display settings appropriately.  Please\n"
			L"let Jason know at email@domain.com.  Thank you!",
			L"Technical Difficulties", MB_ICONEXCLAMATION | MB_OK);
		return false;
	}

	// Create our window.
	Graphics::getInstance().createWindow(hInstance, nCmdShow, ExperimentMain::WndProc);

	// Initialize graphics.
	Graphics::getInstance().initialize();

	// Initialize raw input.
    RAWINPUTDEVICE keyboardRID;
    keyboardRID.usUsage = 0x06;    // register keyboard
    keyboardRID.usUsagePage = 0x01;    // top-level mouse (?)
    keyboardRID.dwFlags = RIDEV_NOLEGACY;    // flags
    keyboardRID.hwndTarget = Graphics::getInstance().hWnd;    // handle to a window
    RegisterRawInputDevices(&keyboardRID, 1, sizeof(RAWINPUTDEVICE));    // register the device

	// Begin the output file.
	Util::getInstance().outputSS << "<experiment-data>" << std::endl;
	Util::getInstance().outputSS << "<subject-id>" << Util::getInstance().subjectId << "</subject-id>" << std::endl;

	// Get the current date and time, in UTC.
	SYSTEMTIME st;
	GetSystemTime(&st);

	// Write our data to the output file.
	/* Util::getInstance().outputSS << "<datetime-utc>" << endl;
	Util::getInstance().outputSS << "<year>" << st.wYear << "</year>" << endl;
	Util::getInstance().outputSS << "<month>" << st.wMonth << "</month>" << endl;
	Util::getInstance().outputSS << "<day>" << st.wDay << "</day>" << endl;
	Util::getInstance().outputSS << "<hour>" << st.wHour << "</hour>" << endl;
	Util::getInstance().outputSS << "<minute>" << st.wMinute << "</minute>" << endl;
	Util::getInstance().outputSS << "<second>" << st.wSecond << "</second>" << endl;
	Util::getInstance().outputSS << "</datetime-utc>" << endl; */

	// Write condition randomization for this run to our file.
	Util::getInstance().outputSS << "<condition-randomization>" << std::endl;
	Util::getInstance().outputSS << "<game-instructions>" << Util::getInstance().randomization_game_instructions
		<< "</game-instructions>" << std::endl;
	Util::getInstance().outputSS << "<game-wordset>" << Util::getInstance().randomization_game_wordset
		<< "</game-wordset>" << std::endl;
	Util::getInstance().outputSS << "<priming-keymap>" << Util::getInstance().randomization_priming_keymap
		<< "</priming-keymap>" << std::endl;
	Util::getInstance().outputSS << "</condition-randomization>" << std::endl;
	
	// Initialize current module index.
	currentModuleIndex = 0;

	// Create modules:
	if (Util::getInstance().replicateDG)
	{
		// Priming practice.
		modules.push_back(make_shared<PrimingModule>(false, true, true, 20, 0));  // 20
		modules.push_back(make_shared<PrimingModule>(true, true, true, 50, 1));  // 50
		modules.push_back(make_shared<PrimingModule>(true, true, true, 50, 2));  // 50
		modules.push_back(make_shared<PrimingModule>(true, false, false, 20, 3));  // 20 // For now, use real words (not practice)
		modules.push_back(make_shared<PrimingModule>(true, false, false, 50, 4));  // 50 // For now, use real words (not practice)
		modules.push_back(make_shared<PrimingModule>(true, false, false, 50, 4));  // 50 // For now, use real words (not practice)

		// First priming block.
		modules.push_back(make_shared<PrimingModule>(true, false, false, 50, 4));  // 50
		modules.push_back(make_shared<PrimingModule>(true, false, false, 50, 4));  // 50
		modules.push_back(make_shared<PrimingModule>(true, false, false, 50, 4));  // 50

		// Two more priming blocks.
		for (int i = 0; i < 2; i++)
		{
			// Priming.
			modules.push_back(make_shared<PrimingModule>(true, false, false, 50, 4));  // 50
			modules.push_back(make_shared<PrimingModule>(true, false, false, 50, 4));  // 50
			modules.push_back(make_shared<PrimingModule>(true, false, false, 50, 4));  // 50
		}

		// Finish and close data files.
		modules.push_back(make_shared<ExecuteCodeModule>());

		// Upload results to the server.
		if (Util::getInstance().uploadResults) modules.push_back(make_shared<UploadModule>());
	}
	else
	{
		// Consent.
		modules.push_back(make_shared<ConsentModule>());

		// Priming practice.
		modules.push_back(make_shared<PrimingModule>(false, true, true, 20, 0));  // 20
		modules.push_back(make_shared<PrimingModule>(true, true, true, 50, 1));  // 50
		modules.push_back(make_shared<PrimingModule>(true, true, true, 50, 2));  // 50
		modules.push_back(make_shared<PrimingModule>(true, false, false, 20, 3));  // 20 // For now, use real words (not practice)
		modules.push_back(make_shared<PrimingModule>(true, false, false, 50, 4));  // 50 // For now, use real words (not practice)
		modules.push_back(make_shared<PrimingModule>(true, false, false, 50, 4));  // 50 // For now, use real words (not practice)

		// First game module.
		modules.push_back(make_shared<GameModule>(true));

		// First priming block.
		modules.push_back(make_shared<PrimingModule>(true, false, false, 50, 4));  // 50
		modules.push_back(make_shared<PrimingModule>(true, false, false, 50, 4));  // 50
		modules.push_back(make_shared<PrimingModule>(true, false, false, 50, 4));  // 50

		// Two more game-priming pairs.
		for (int i = 0; i < 2; i++)
		{
			// Game.
			modules.push_back(make_shared<GameModule>(false));

			// Priming.
			modules.push_back(make_shared<PrimingModule>(true, false, false, 50, 4));  // 50
			modules.push_back(make_shared<PrimingModule>(true, false, false, 50, 4));  // 50
			modules.push_back(make_shared<PrimingModule>(true, false, false, 50, 4));  // 50
		}

		// Questionnaires.
		modules.push_back(make_shared<AAQModule>());
		modules.push_back(make_shared<BASModule>());
		modules.push_back(make_shared<GRFModule>());
		modules.push_back(make_shared<EAMModule>());

		// Demographic information.
		modules.push_back(make_shared<DemographicModule>());

		// Finish and close data files.
		modules.push_back(make_shared<ExecuteCodeModule>());

		// Upload results to the server.
		if (Util::getInstance().uploadResults) modules.push_back(make_shared<UploadModule>());

		// Debriefing.
		modules.push_back(make_shared<DebriefingModule>());
	}

	// Set up progress times.
	Util::getInstance().elapsedProgressTime = 0;
	Util::getInstance().totalProgressTime = 0;
	for (auto iter = modules.begin(); iter != modules.end(); ++iter)
	{
		Util::getInstance().totalProgressTime += (*iter)->progressTimeSeconds();
	}

	// We successfully initialized.
	return true;
}


// Clean up.
void ExperimentMain::cleanUp()
{
	// Clean up graphics.
	Graphics::getInstance().cleanUp();

	// Reset display refresh rate, if we changed it.
	Graphics::getInstance().resetDisplayRefreshRate();
}


// Destructor.
ExperimentMain::~ExperimentMain()
{
}


int ExperimentMain::renderFrame()
{
	// Render frame in the appropriate module.
	int moduleStatus = modules[currentModuleIndex]->render_frame();

	// Is module finished?
	if (moduleStatus == Util::MODULE_STATE_FINISHED)
	{
		// Update progress time.
		Util::getInstance().elapsedProgressTime += modules[currentModuleIndex]->progressTimeSeconds();

		// Is the experiment finished?
		if (currentModuleIndex == modules.size() - 1)
		{
			// Experiment is finished.
			return Util::EXPERIMENT_STATE_FINISHED;
		}
		else
		{
			// Go to the next module.
			currentModuleIndex++;
		}
	}

	// Experiment is still running.
	return Util::EXPERIMENT_STATE_RUNNING;
}


// This is the main message handler for the program
LRESULT CALLBACK ExperimentMain::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
    {
        case WM_DESTROY: { PostQuitMessage(0); return 0; } break;
        case WM_INPUT: {

			// Get input data.
            RAWINPUT InputData;
			UINT DataSize = sizeof(RAWINPUT);
            GetRawInputData((HRAWINPUT)lParam, RID_INPUT, &InputData, &DataSize, sizeof(RAWINPUTHEADER));

			// Get input specifics.
			USHORT vKey = InputData.data.keyboard.VKey;
			bool isDown = (InputData.data.keyboard.Message == WM_KEYDOWN ? true : false);
			bool isUp = (InputData.data.keyboard.Message == WM_KEYUP ? true : false);

			// Get a reference to the experiment main (singleton) instance.
			ExperimentMain& instance = ExperimentMain::getInstance();

			// If escape key, quit the program.
			if (vKey == VK_ESCAPE && isDown) { 
				PostQuitMessage(0); 
				return 0; 
			}

			// If 'F11' key, move to the next module (or quit if we're on the last module).
			else if (vKey == VK_F11 && isDown)
			{ 
				// Update progress time.
				Util::getInstance().elapsedProgressTime += instance.modules[instance.currentModuleIndex]->progressTimeSeconds();

				if (instance.currentModuleIndex == instance.modules.size() - 1) { PostQuitMessage(0); return 0; }
				else { instance.currentModuleIndex++; return 0; }
			}

			// Otherwise, allow current module to handle the input.
			else return instance.modules[instance.currentModuleIndex]->handleInput(InputData);

        } break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}




