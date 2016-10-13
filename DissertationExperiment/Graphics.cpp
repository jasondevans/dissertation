#include "stdafx.h"

#include "Graphics.h"
#include "Util.h"

// Library files.
#pragma comment (lib, "d2d1.lib")
#pragma comment (lib, "dwrite.lib")


// Attempt to set the display refresh rate.
int Graphics::setDisplayRefreshRate(DWORD toRate)
{
	// We can't handle multiple calls yet (not the best, but for now just return success for subsequent calls).
	if (changedDevMode) return 0;

	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &currentDevMode);
	if (currentDevMode.dmDisplayFrequency < (toRate - 4) || currentDevMode.dmDisplayFrequency > (toRate + 4))
	{
		vector<DEVMODE> candidates;
		for (auto iter = devModes.begin(); iter != devModes.end(); ++iter)
		{
			// Get all acceptable display settings
			if (iter->dmDisplayFrequency > (toRate - 4) && iter->dmDisplayFrequency < (toRate + 4)
				&& iter->dmPelsWidth >=640 && iter->dmPelsHeight >= 480
				&& iter->dmPosition.x == 0 && iter->dmPosition.y == 0)
			{
				candidates.push_back(*iter);
			}
		}

		// Sort acceptable display settings by desirability.
		struct dev_mode_less_than
		{
			inline bool operator() (const DEVMODE& devMode1, const DEVMODE& devMode2)
			{
				DEVMODE cDevMode;
				EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &cDevMode);
				if (devMode1.dmPelsWidth != devMode2.dmPelsWidth)
				{
					if (devMode1.dmPelsWidth == cDevMode.dmPelsWidth) return true;
					if (devMode2.dmPelsWidth == cDevMode.dmPelsWidth) return false;
				}
				if (devMode1.dmPelsHeight != devMode2.dmPelsHeight)
				{
					if (devMode1.dmPelsHeight == cDevMode.dmPelsHeight) return true;
					if (devMode2.dmPelsHeight == cDevMode.dmPelsHeight) return false;
				}
				if (devMode1.dmBitsPerPel != devMode2.dmBitsPerPel)
				{
					if (devMode1.dmBitsPerPel == cDevMode.dmBitsPerPel) return true;
					if (devMode2.dmBitsPerPel == cDevMode.dmBitsPerPel) return false;
				}
				if (devMode1.dmDisplayFlags != devMode2.dmDisplayFlags)
				{
					if (devMode1.dmDisplayFlags == cDevMode.dmDisplayFlags) return true;
					if (devMode2.dmDisplayFlags == cDevMode.dmDisplayFlags) return false;
				}
				if (devMode1.dmDisplayFrequency != devMode2.dmDisplayFrequency)
				{
					if (abs((int) devMode1.dmDisplayFrequency - 60) < abs((int) devMode2.dmDisplayFrequency - 60))
						return true;
					else
						return false;
				}
				if (devMode1.dmPelsWidth > devMode2.dmPelsWidth) return true;
				if (devMode1.dmPelsWidth < devMode2.dmPelsWidth) return false;
				if (devMode1.dmPelsHeight > devMode2.dmPelsHeight) return true;
				if (devMode1.dmPelsHeight < devMode2.dmPelsHeight) return false;
				if (devMode1.dmBitsPerPel > devMode2.dmBitsPerPel) return true;
				if (devMode1.dmBitsPerPel < devMode2.dmBitsPerPel) return false;
				if (devMode1.dmDisplayFlags < devMode2.dmDisplayFlags) return true;
				if (devMode1.dmDisplayFlags > devMode2.dmDisplayFlags) return false;
				return false;
			}
		};
		sort(candidates.begin(), candidates.end(), dev_mode_less_than());
		bool changeHzSuccess = false;
		if (candidates.size() > 0)
		{
			// Change display settings here.
			newDevMode = candidates[0];
			int displayChangeResult = ChangeDisplaySettings(&newDevMode, CDS_FULLSCREEN);
			if (displayChangeResult == DISP_CHANGE_SUCCESSFUL)
			{
				changeHzSuccess = true;
				changedDevMode = true;
			}
		}
		if (!changeHzSuccess)
		{
			return -1;
		}
	}
	
	// If we get here, success.
	return 0;
}


// Create a window.
void Graphics::createWindow(HINSTANCE hInstance, int nCmdShow, WNDPROC WindowProc)
{
    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    if (!Util::getInstance().fullScreen) wc.hbrBackground = (HBRUSH)COLOR_WINDOW;  // Comment this line out for full-screen.
    wc.lpszClassName = L"WindowClass";

    RegisterClassEx(&wc);

	RECT wr = {0, 0, displayWidth, displayHeight};    // set the size, but not the position
	if (!Util::getInstance().fullScreen)
	{
		AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);    // adjust the size
	}

    hWnd = CreateWindowEx(NULL,
        L"WindowClass",
        L"Experiment",
		Util::getInstance().fullScreen ? WS_EX_TOPMOST | WS_POPUP : WS_OVERLAPPEDWINDOW,
        0, 0,
		wr.right - wr.left,    // width of the window
		wr.bottom - wr.top,    // height of the window
        NULL,
        NULL,
        hInstance,
        NULL);

    ShowWindow(hWnd, nCmdShow);
	ShowCursor(FALSE);
}


// Initialize.
void Graphics::initialize()
{
	// Declare a result object.
	HRESULT hr = S_OK;

	// Create a Direct2D factory.
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2dFactory);

	// Create a DirectWrite factory.
	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&dwriteFactory));

	// Get display dimensions.
	RECT rc;
    GetClientRect(hWnd, &rc);
	D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

	// Create a Direct2D render target.
    hr = d2dFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(hWnd, size), &renderTarget);

	// Create a brush.
    hr = renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &brush);
	
	// Create a standard font.
	float standardFontSize = (float) (displayHeight / 36);
	standardFont = new Font(standardFontSize, L"Tahoma");

	// Create a word font.
	float wordFontSize = (float) (displayHeight / 18);
	wordFont = new Font(wordFontSize, L"Tahoma");
}


// Clean up graphics.
void Graphics::cleanUp()
{
	// Release DirectX objects.
	SafeRelease(&d2dFactory);
    SafeRelease(&renderTarget);
    SafeRelease(&brush);
	SafeRelease(&dwriteFactory);

	// Release fonts.
	if (standardFont) SafeRelease(&(standardFont->impl));
	if (wordFont) SafeRelease(&(wordFont->impl));
}


// Reset display refresh rate to original value.
void Graphics::resetDisplayRefreshRate()
{
	// If we changed display settings, set them back to original value.
	if (changedDevMode)
	{
		int displayChangeResult = ChangeDisplaySettings(&currentDevMode, CDS_FULLSCREEN);
	}
}


// Measure the size of the rectangle drawing the given string will occupy.
RECT Graphics::measureString(Font* font, string str)
{
	IDWriteTextLayout* textLayout = nullptr;

	wstring wstr = Util::s2ws(str);
	const wchar_t* wstrarr = wstr.c_str();
	int length = wstr.length();

	dwriteFactory->CreateTextLayout(
		wstrarr,      // The string to be laid out and formatted.
		length,  // The length of the string.
		font->impl,  // The text format to apply to the string (contains font information, etc).
		(float) displayWidth,         // The width of the layout box.
		(float) displayHeight,        // The height of the layout box.
		&textLayout  // The IDWriteTextLayout interface pointer.
		);

	DWRITE_TEXT_METRICS textMetrics;
	textLayout->GetMetrics(&textMetrics);

	RECT result;
	result.left = (long) textMetrics.left;
	result.top = (long) textMetrics.top;
	result.right = (long) textMetrics.width;
	result.bottom = (long) textMetrics.height;

	// Release resource.
	SafeRelease(&textLayout);

	return result;
}


// Draw text to the screen.
void Graphics::drawText(string text, int x, int y, Font* font, Color color)
{
	// Create a RECT to contain the text.
	D2D1_RECT_F renderRect;
	renderRect.left = (float) x;
	renderRect.top = (float) y;
	renderRect.right = (float) displayWidth;
	renderRect.bottom = (float) displayHeight;

	// Set color.
	brush->SetColor(color.impl);

	// Draw the text.
	renderTarget->DrawText(Util::s2ws(text).c_str(), text.length(), font->impl, renderRect, brush);
}


// Draw text centered on the screen.
void Graphics::drawTextCentered(string text, Font* font, Color color)
{
	static RECT textDimensions;
	textDimensions = measureString(font, text);
	int textWidth = textDimensions.right - textDimensions.left;
	int textHeight = textDimensions.bottom - textDimensions.top;
	int positionX = (displayWidth - textWidth) / 2;
	int positionY = (displayHeight - textHeight) / 2;

	// Draw the text.
	drawText(text, positionX, positionY, font, color);
}


// Draw a rectangle.
void Graphics::drawRectangle(RECT rect, Color color)
{
	// Create drawing rectangle.
	D2D1_RECT_F rectangle = 
		D2D1::RectF((FLOAT) rect.left, (FLOAT) rect.top, (FLOAT) rect.right, (FLOAT) rect.bottom);

	// Set color.
	brush->SetColor(color.impl);
	
    // Draw a filled rectangle.
    renderTarget->FillRectangle(&rectangle, brush);
}


// Draw a line.
void Graphics::drawLine(RECT rect, Color color)
{
	// Set brush color.
	brush->SetColor(color.impl);

	// Draw the line.
	renderTarget->DrawLine(D2D1::Point2F(static_cast<FLOAT>(rect.left), static_cast<FLOAT>(rect.top)),
		D2D1::Point2F(static_cast<FLOAT>(rect.right), static_cast<FLOAT>(rect.bottom)), brush, 1.0f, NULL);
}


// Draw progress text.
void Graphics::drawProgressText()
{
	static RECT textDimensions;
	static stringstream sstm;
	static string text;
	sstm.str("");
	sstm << "Progress: " << (100 * Util::getInstance().elapsedProgressTime / Util::getInstance().totalProgressTime) << "%";
	text = sstm.str();
	textDimensions = measureString(standardFont, text);
	int textWidth = textDimensions.right - textDimensions.left;
	int textHeight = textDimensions.bottom - textDimensions.top;
	int positionX = displayWidth - textWidth - textWidth / 5;
	int positionY = textWidth / 6;

	// Draw the text.
	drawText(text, positionX, positionY, standardFont, Color(255, 255, 255));
}


// Set up a new frame.
void Graphics::setUpNewFrame()
{
	renderTarget->BeginDraw();

    renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

    renderTarget->Clear(standardBackgroundColor.impl);

	/*
	// Clear the window.
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, standardBackgroundColor.impl, 1.0f, 0);

	// Begin the 3D scene.
	d3ddev->BeginScene();
	*/
}

// Finish and present a frame.
void Graphics::finishAndPresentFrame()
{
	renderTarget->EndDraw();

	/*
	// End the 3D scene.
	d3ddev->EndScene();

	// Display the created frame on the screen.
	d3ddev->Present(NULL, NULL, NULL, NULL);
	*/
}


// Constructor.
Graphics::Graphics()
{ 
	// Get the current devmode, and a vector of all devmodes.
	int result = 1;
	int iModeNum = 0;
	while (result != 0)
	{
		DEVMODE thisDevMode;
		result = EnumDisplaySettings(NULL, iModeNum, &thisDevMode);
		if (result != 0) devModes.push_back(thisDevMode);
		iModeNum++;
	}

	// We haven't changed our display devmode yet.
	changedDevMode = false;

	/*
	// DirectX declarations.
	d3d = NULL;
	d3ddev = NULL;
	*/

	// Initialize DirectX variables.
	d2dFactory = NULL;
    renderTarget = NULL;
    brush = NULL;

	// Set display dimensions.
	if (Util::getInstance().fullScreen)
	{
		displayWidth = GetSystemMetrics(SM_CXSCREEN);
		displayHeight = GetSystemMetrics(SM_CYSCREEN);
	}
	else
	{
		displayWidth = 800; 
		displayHeight = 600;
	}

	// Set the standard background color.
	standardBackgroundColor = Color(50, 50, 50, 255); // Near-black
}


// Destructor.
Graphics::~Graphics()
{
	cleanUp();
	/*
	delete standardFont;
	delete wordFont;
	*/
}





