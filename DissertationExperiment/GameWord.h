#pragma once

#include "Color.h"

using namespace std;


class GameWord
{

public:

	// The word itself.
	string wordText;

	// Current color for this word.
	Color currentColor;

	// Flash-on color for this word.
	Color flashOnColor;

	// Whether we should flash the color.
	bool flashColor;

	// Constructor.
	GameWord();

	// Constructor.
	GameWord(string wordText);

};