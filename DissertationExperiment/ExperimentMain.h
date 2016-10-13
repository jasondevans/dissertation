#pragma once
#include "stdafx.h"
#include "Module.h"
#include <memory>


class ExperimentMain
{

private:

	int currentModuleIndex;

public:

	// Our modules.
	std::vector<std::shared_ptr<Module>> modules;

	// Constructor.
	ExperimentMain();

	// Return a reference to our singleton.
	static ExperimentMain& getInstance()
    {
        static ExperimentMain instance;
        return instance;
    }

	// Destructor.
	~ExperimentMain();

	// Initialize the experiment.
	// Returns true if succeeded, false if failed.
	bool initialize(HINSTANCE hInstance, int nCmdShow);

	// Clean up.
	void cleanUp();

	// Render a single frame.
	int renderFrame();

    // The window message processing function (e.g. input, etc.).
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

};