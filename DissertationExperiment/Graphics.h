#pragma once

#include "stdafx.h"
#include "Font.h"
#include "Color.h"

// DirectX headers
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>

using namespace std;


template<typename Interface>
inline void SafeRelease(
    Interface **ppInterfaceToRelease
    )
{
    if (*ppInterfaceToRelease != NULL)
    {
        (*ppInterfaceToRelease)->Release();

        (*ppInterfaceToRelease) = NULL;
    }
}


class Graphics
{

public:

	// Window declarations.
	HWND hWnd;
    WNDCLASSEX wc;

	// DirectX declarations.
	ID2D1Factory* d2dFactory;
    ID2D1HwndRenderTarget* renderTarget;
    ID2D1SolidColorBrush* brush;
	IDWriteFactory* dwriteFactory;
	
	// Display width.
	int displayWidth;

	// Display height.
	int displayHeight;

	// Standard font
	Font* standardFont;

	// Font for displaying words.
	Font* wordFont;

    // Standard background color.
    Color standardBackgroundColor;

	// Attempt to set the display refresh rate.
	int setDisplayRefreshRate(DWORD toRate);

	// Create a window.
	void createWindow(HINSTANCE hInstance, int nCmdShow, WNDPROC WindowProc);

	// Initialize graphics.
	void initialize();

	// Clean up graphics.
	void cleanUp();

	// Reset display refresh rate to original value.
	void resetDisplayRefreshRate();

	// Measure the size of the rectangle drawing the given string will occupy.
	RECT measureString(Font* font, string str);

	// Draw text to the screen.
	void drawText(string text, int x, int y, Font* font, Color color);

	// Draw text centered on the screen.
	void drawTextCentered(string text, Font* font, Color color);

	// Draw a rectangle.
	void drawRectangle(RECT rect, Color color);

	// Draw a line.
	void drawLine(RECT rect, Color color);

	// Draw progress text.
	void drawProgressText();

	// Set up a new frame.
	void setUpNewFrame();

	// Finish and present a frame.
	void finishAndPresentFrame();

	// Constructor.
	Graphics();
	
	// Return a reference to our singleton.
	static Graphics& getInstance()
    {
        static Graphics instance;
        return instance;
    }

	// Destructor.
	~Graphics();

private:

	// Display device modes
	vector<DEVMODE> devModes;
	DEVMODE currentDevMode;
	DEVMODE newDevMode;
	bool changedDevMode;

	/*
	// Vector of vertex buffers.
	vector<LPDIRECT3DVERTEXBUFFER9> vertexBuffers;

	// Create a vertex buffer for the avatar's vertices.
	void createVertexBuffer(LPDIRECT3DVERTEXBUFFER9* buffer, int size);
	*/

};
