#pragma once

#include "WordSet.h"
#include "DroppingWord.h"
#include "Timer.h"
#include "Module.h"
#include "GameWord.h"
#include "Util.h"
#include "Color.h"
#include "Qubit.h"
#include "StateManager.h"


class GameModule : public Module
{

public:

	// Keep track of our Qubit's, so we can free memory later.
	vector<Qubit*> qubits;

	// Our state manager.
	StateManager stateManager;

	// Avatar width and height (in pixels).
	int avatarWidth;
	int avatarHeight;

	// The avatar position.
	float avatarX;
	float avatarY;

	// How many pixels the avatar should move per frame.
	float avatarVelocityLeft;
	float avatarVelocityRight;
	float avatarVelocityStopped;

	// Our sets of game words.
	map<string, GameWord> firstGameWordsSet;
	map<string, GameWord> secondGameWordsSet;

	// Which set of game words to use at the current time.
	int whichGameWordsSet;
	static const int GAME_WORDS_SET_BOTH = 0;
	static const int GAME_WORDS_SET_FIRST = 1;
	static const int GAME_WORDS_SET_SECOND = 2;

	// Word colors.
	Color firstColorSet1;
	Color firstColorSet2;
	Color firstColorSet3;
	Color secondColorSet1;
	Color secondColorSet2;
	Color secondColorSet3;

	// Current word color variables.
	Color firstColor;
	Color firstColorFlash;
	bool firstFlash;
	Color secondColor;
	Color secondColorFlash;
	bool secondFlash;

	// Whether flashing words are "on" or "off".
	bool flashIsOn; // Whether flashing words are "on" or "off".

	// Dropping velocity variables, in pixels per frame.
	float droppingVelocityMaxEasy;
	float droppingVelocityMaxHard;
	float droppingVelocityProportion;  // from 0.0f to 1.0f ranging from easy to hard.

	// Frames between introducing a new dropping word.
	int framesBetweenWordsMaxEasy;
	int framesBetweenWordsMaxHard;
	float framesBetweenWordsProportion;  // from 0.0f to 1.0f ranging from easy to hard.

	// The number of frames since the last new word started dropping.
	int framesSinceNewWord;

	// The radius within which to display new words (relative to previous word),
	// in proportion of effective screen width.
	float displayRadiusMaxEasy;
	float displayRadiusMaxHard;
	float displayRadiusProportion;  // from 0.0f to 1.0f ranging from easy to hard.

	// Which word set we used last, and how many repeats of the same set.
	bool lastWordSet; // true = first word set; false = second word set
	int wordSetRepeatNum;

	// Remember the last two words.
	DroppingWord lastWord;
	DroppingWord lastLastWord;

	// Word delay offset in millis.
	int wordDelayOffsetMillis;

	// List of words currently on the screen.
	list<DroppingWord> currentWords;

	// The max and min positions for the avatar.
	int avatarMinX, avatarMaxX;

	// Our timers for the current dropping words game.
	Timer timer;
	Timer flashTimer;

	// The duration of each phase of the game.
	long phaseDurationMillis;

	// Keyboard states.
	bool leftArrow;
	bool rightArrow;
	
	// Constructor
	GameModule(bool firstRun);

	// Destructor.
	~GameModule();

	// Set up phases for the first run.
	virtual void setUpPhasesFirstRun();

	// Set up phases for subsequent runs.
	virtual void setUpPhases();

	// Render a frame.
	virtual int render_frame();

	// Handle keyboard input.
	virtual LRESULT handleInput(RAWINPUT InputData);

	LRESULT handleInputGame(RAWINPUT InputData);

	virtual int progressTimeSeconds();

	int render_frame_game();

	void resetState();

	void writeData();

	// Initialization functions.
	void initializeFirstSet();
	void initializeSecondSet();
	void initializeBothSets();
	void initializeBothSetsReverseColor();
	void initializeBothSetsLowContrast();
	void initializeBothSetsLowContrastReverseColor();
	void initializeBothSetsFlash();
	void initializeBothSetsSameColor();

private:

	void updateState();

	void addNewWords();

	void drawWords();

	void drawAvatar();

	void drawTopBar();

	bool rectanglesOverlap(int x1, int y1, int xwidth1, int ywidth1, int x2, int y2, int xwidth2, int ywidth2);

	bool avatarTouchingWords();

	void setWordAttributes(Color firstSetCurrentColor, Color firstSetFlashColor, bool firstSetFlash, 
		Color secondSetCurrentColor, Color secondSetFlashColor, bool secondSetFlash);

	float getProportionValue(float proportion, float minValue, float maxValue);

};