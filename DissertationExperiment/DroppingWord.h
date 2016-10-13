#pragma once


using namespace std;


class DroppingWord
{

public:

	string wordText;
	bool shouldAvoid;
	int positionX;
	int positionY;
	int startDelayMillis;
	int screenWidth;
	int screenHeight;
	bool wasTouchedByAvatar;

	// Whether this word is a part of "Set A" (as opposed to "Set B").
	bool isFromSetA;

	// Constructor.
	DroppingWord();

};