// Include shared header file.
#include "stdafx.h"

// Include our header file.
#include "GameModule.h"
#include "WordSet.h"
#include "DroppingWord.h"
#include "Util.h"
#include "Graphics.h"
#include "Color.h"
#include "State.h"
#include "DisplayUntilKeypressQubit.h"
#include "DisplayForFramesQubit.h"


// String equals.
bool stringEquals(string& one, string& two);
	

// Constructor.
GameModule::GameModule(bool firstRun)
{
	// Set keyboard state.
	leftArrow = false;
	rightArrow = false;

	// Which word set we used last, and how many repeats of the same set.
	lastWordSet = Util::getInstance().randomInt(0, 2) == 0 ? true : false;
	wordSetRepeatNum = 0;

	// Initial value for number of frames since last new word started dropping.
	framesSinceNewWord = 1000;

	// Set word flashing initial value.
	flashIsOn = false;

	// Set up colors, depending on randomization.
	bool gameColorSchemeThisRun = Util::getInstance().randomInt(0, 2) == 0 ? true : false;
	if (gameColorSchemeThisRun)
	{
		firstColorSet1 = Color(123, 224, 220);
		firstColorSet2 = Color(222, 250, 229);
		firstColorSet3 = Color(255, 255, 255);
		secondColorSet1 = Color(255, 208, 120);
		secondColorSet2 = Color(255, 223, 191);
		secondColorSet3 = Color(255, 255, 255);
	}
	else
	{
		secondColorSet1 = Color(123, 224, 220);
		secondColorSet2 = Color(222, 250, 229);
		secondColorSet3 = Color(255, 255, 255);
		firstColorSet1 = Color(255, 208, 120);
		firstColorSet2 = Color(255, 223, 191);
		firstColorSet3 = Color(255, 255, 255);
	}

	// Set avatar width and height.
	avatarWidth = Graphics::getInstance().displayWidth / 6;
	avatarHeight = Graphics::getInstance().displayHeight / 20;

	// Determine avatar velocities (in pixels of movement per frame).
	avatarVelocityLeft = - ((float) Graphics::getInstance().displayWidth) / 60.0f;
	avatarVelocityRight = ((float) Graphics::getInstance().displayWidth) / 60.0f;
	avatarVelocityStopped = 0.0f;

	// Set up dropping velocity variables.
	droppingVelocityMaxEasy = Graphics::getInstance().displayHeight * 0.005f;
	droppingVelocityMaxHard = Graphics::getInstance().displayHeight * 0.014f;
	droppingVelocityProportion = firstRun ? 0.0f : 0.75f;

	// Set up frames between words variables.
	framesBetweenWordsMaxEasy = 50;
	framesBetweenWordsMaxHard = 23;
	framesBetweenWordsProportion = firstRun ? 0.0f : 0.75f;

	// The radius within which to display new words (relative to previous word),
	// in proportion of effective screen width.
	displayRadiusMaxEasy = 0.30f;
	displayRadiusMaxHard = 0.60f;
	displayRadiusProportion = firstRun ? 0.0f : 0.75f;

	// Calculate max and min positions for the avatar.
	avatarMinX = - (avatarWidth * 3 / 4);
	avatarMaxX = Graphics::getInstance().displayWidth - (avatarWidth / 4);
	
	// Set initial avatar position.
	avatarX = (Graphics::getInstance().displayWidth - avatarWidth) / 2.0f;
	avatarY = (float) Graphics::getInstance().displayHeight - avatarHeight;

	// The duration of each "phase" of the game.
	phaseDurationMillis = 20000;

	// Create game word sets.
	for (auto iter = Util::getInstance().firstGameWords.begin(); iter != Util::getInstance().firstGameWords.end(); ++iter)
	{
		GameWord thisGameWord((*iter));
		firstGameWordsSet[*iter] = thisGameWord;
	}
	for (auto iter = Util::getInstance().secondGameWords.begin(); iter != Util::getInstance().secondGameWords.end(); ++iter)
	{
		GameWord thisGameWord((*iter));
		secondGameWordsSet[*iter] = thisGameWord;
	}

	// Set up the game phases appropriately.
	if (firstRun)
	{
		setUpPhasesFirstRun();
	}
	else
	{
		setUpPhases();
	}
}


void GameModule::setUpPhasesFirstRun()
{
	// Instructions preliminaries.
	string instrHeader = "========== VIDEO GAME ==========\n\n";
	string instrFooter = "\n\n\nPress any key to continue >>>>>";
	string instrA;
	string instrB;
	Qubit* pThisQubit;
	State* pThisState;

	// Instructions.
	instrA = instrHeader + ""
		"On the following screen, you will see a series of words falling from the top\n"
		"of the screen to the bottom of the screen.  The falling words will be:\n\n"
		"" + Util::getInstance().firstGameWords[0] + " and " + Util::getInstance().firstGameWords[1] + "\n\n"
		"At the bottom of the screen will be a small platform that can move left and\n"
		"right, which you can control by pressing the left and right arrow keys.\n\n"
		"Your task is to move the platform so that it TOUCHES each falling word.  When\n"
		"the platform touches a word, it will slightly change color."
		"" + instrFooter;
	instrB = instrHeader + ""
		"On the following screen, you will see a series of words falling from the top\n"
		"of the screen to the bottom of the screen.  The falling words will be:\n\n"
		"" + Util::getInstance().firstGameWords[0] + " and " + Util::getInstance().firstGameWords[1] + "\n\n"
		"At the bottom of the screen will be a small platform that can move left and\n"
		"right, which you can control by pressing the left and right arrow keys.\n\n"
		"Imagine the falling words are like bubbles.  Your task is to AVOID letting\n"
		"the falling words touch the ground by \"popping\" them, by bringing the\n"
		"platform into contact with the words so that it touches them.  When the\n"
		"platform touches a word, the platform will slightly change color.  (Note\n"
		"that you won't actually see any change in the words when you touch them\n"
		"with the platform.)"
		"" + instrFooter;
	pThisQubit = new DisplayUntilKeypressQubit(Util::getInstance().randomization_game_instructions ? instrA : instrB, 34, true);
	qubits.push_back(pThisQubit);
	pThisState = new State(*pThisQubit);
	pThisState->cleanupFn = boost::bind(&GameModule::resetState, boost::ref(*this));
	stateManager.states.push_back(pThisState);

	// Add a "first set" phase.
	pThisState = new State(boost::bind(&GameModule::render_frame_game, boost::ref(*this)),
		boost::bind(&GameModule::handleInputGame, boost::ref(*this), _1));
	pThisState->initializeFn = boost::bind(&GameModule::initializeFirstSet, boost::ref(*this));
	stateManager.states.push_back(pThisState);

	// Transition screen.
	pThisQubit = new DisplayForFramesQubit("", "", 34, Graphics::getInstance().standardFont);
	qubits.push_back(pThisQubit);
	pThisState = new State(*pThisQubit);
	stateManager.states.push_back(pThisState);
	
	instrA = instrHeader + ""
		"Now you will see a series of new falling words:\n\n"
		"" + Util::getInstance().secondGameWords[0] + " and " + Util::getInstance().secondGameWords[1] + "\n\n"
		"This time, your task is to move the platform so that it AVOIDS these words.\n"
		"Again, if the platform touches a word, it will slightly change color -- but\n"
		"the goal is to avoid touching these words as much as possible."
		"" + instrFooter;
	instrB = instrHeader + ""
		"Now you will see a series of new falling words:\n\n"
		"" + Util::getInstance().secondGameWords[0] + " and " + Util::getInstance().secondGameWords[1] + "\n\n"
		"Again, imagine these falling words are like bubbles.  However, this time\n"
		"your task is to allow the words to touch the ground by NOT \"popping\"\n"
		"them -- by allowing them to touch the ground.  You do this by moving the\n"
		"platform so that it does NOT touch these words. Again, if the platform\n"
		"touches a word, the platform will slightly change color -- but the goal\n"
		"is to not touch (not \"pop\") these words."
		"" + instrFooter;
	pThisQubit = new DisplayUntilKeypressQubit(Util::getInstance().randomization_game_instructions ? instrA : instrB, 34);
	qubits.push_back(pThisQubit);
	pThisState = new State(*pThisQubit);
	pThisState->cleanupFn = boost::bind(&GameModule::resetState, boost::ref(*this));
	stateManager.states.push_back(pThisState);

	// Add a "second set" phase.
	pThisState = new State(boost::bind(&GameModule::render_frame_game, boost::ref(*this)),
		boost::bind(&GameModule::handleInputGame, boost::ref(*this), _1));
	pThisState->initializeFn = boost::bind(&GameModule::initializeSecondSet, boost::ref(*this));
	stateManager.states.push_back(pThisState);

	// Transition screen.
	pThisQubit = new DisplayForFramesQubit("", "", 34, Graphics::getInstance().standardFont);
	qubits.push_back(pThisQubit);
	pThisState = new State(*pThisQubit);
	stateManager.states.push_back(pThisState);
	
	instrA = instrHeader + ""
		"Now there will be falling words from both sets.  Your task is to move the\n"
		"platform so that it touches one set of words and avoids touching the other\n"
		"set of words.\n\n"
		"You should TOUCH these words:  " + Util::getInstance().firstGameWords[0] + " and " + Util::getInstance().firstGameWords[1] + "\n\n"
		"You should AVOID these words:  " + Util::getInstance().secondGameWords[0] + " and " + Util::getInstance().secondGameWords[1] + "\n\n"
		"To help you remember, the words to approach and avoid will be shown at the\n"
		"top of the screen.\n\n"
		"The words to approach will be in one color, and the words to avoid will be\n"
		"in another color."
		"" + instrFooter;
	instrB = instrHeader + ""
		"Now there will be falling words from both sets.  Your task is to move the\n"
		"platform so that it \"pops\" (touches) one set of words to keep them from\n"
		"reaching the ground, while allowing the other set of words.\n\n"
		"You should POP these words:  " + Util::getInstance().firstGameWords[0] + " and " + Util::getInstance().firstGameWords[1] + "\n\n"
		"You should ALLOW these words:  " + Util::getInstance().secondGameWords[0] + " and " + Util::getInstance().secondGameWords[1] + "\n\n"
		"To help you remember, the words to pop and allow will be shown at the top\n"
		"of the screen.\n\n"
		"The words to pop will be in one color, and the words to allow will be\n"
		"in another color."
		"" + instrFooter;
	pThisQubit = new DisplayUntilKeypressQubit(Util::getInstance().randomization_game_instructions ? instrA : instrB, 34);
	qubits.push_back(pThisQubit);
	pThisState = new State(*pThisQubit);
	pThisState->cleanupFn = boost::bind(&GameModule::resetState, boost::ref(*this));
	stateManager.states.push_back(pThisState);

	// Add a "both sets" phase.
	pThisState = new State(boost::bind(&GameModule::render_frame_game, boost::ref(*this)),
		boost::bind(&GameModule::handleInputGame, boost::ref(*this), _1));
	pThisState->initializeFn = boost::bind(&GameModule::initializeBothSets, boost::ref(*this));
	stateManager.states.push_back(pThisState);

	// Transition screen.
	pThisQubit = new DisplayForFramesQubit("", "", 34, Graphics::getInstance().standardFont);
	qubits.push_back(pThisQubit);
	pThisState = new State(*pThisQubit);
	stateManager.states.push_back(pThisState);
	
	instrA = instrHeader + ""
		"Now there will again be falling words from both sets, and your task is to move the\n"
		"platform so that it touches one set of words and avoids touching the other\n"
		"set of words.  The words are the same as before:\n\n"
		"You should TOUCH these words:  " + Util::getInstance().firstGameWords[0] + " and " + Util::getInstance().firstGameWords[1] + "\n\n"
		"You should AVOID these words:  " + Util::getInstance().secondGameWords[0] + " and " + Util::getInstance().secondGameWords[1] + "\n\n"
		"Initially, the words to approach and avoid will be in different colors, as before.\n"
		"However, the colors of the words will change periodically -- they may reverse, \n"
		"become harder to distinguish, or both sets may even be the same color, as the \n"
		"difficulty of the game increases.\n\n"
		"Regardless of the colors, your task is always the same -- to approach/touch one set\n"
		"of words and avoid touching the other set.  The words will remain the same.\n\n"
		"Again, the words to approach / avoid will be shown at the top of the screen to help\n"
		"you remember."
		"" + instrFooter;
	instrB = instrHeader + ""
		"Now there will again be falling words from both sets, and your task is to move the\n"
		"platform so that it pops one set of words by touching them, and allows the other set\n"
		"of words to reach the ground by not touching them.  The words are the same as before:\n\n"
		"You should POP these words:  " + Util::getInstance().firstGameWords[0] + " and " + Util::getInstance().firstGameWords[1] + "\n\n"
		"You should ALLOW these words:  " + Util::getInstance().secondGameWords[0] + " and " + Util::getInstance().secondGameWords[1] + "\n\n"
		"Initially, the words to pop and allow will be in different colors, as before.\n"
		"However, the colors of the words will change periodically -- they may reverse,\n"
		"become harder to distinguish, or both sets may even be the same color, as the\n"
		"difficulty of the game increases.\n\n"
		"Regardless of the colors, your task is always the same -- to pop/touch one set of\n"
		"words to prevent them from reaching the ground, and to allow the other set to reach\n"
		"the ground by not touching them.  The words will remain the same.\n\n"
		"Again, the words to pop / allow will be shown at the top of the screen to help\n"
		"you remember."
		"" + instrFooter;
	pThisQubit = new DisplayUntilKeypressQubit(Util::getInstance().randomization_game_instructions ? instrA : instrB, 34);
	qubits.push_back(pThisQubit);
	pThisState = new State(*pThisQubit);
	pThisState->cleanupFn = boost::bind(&GameModule::resetState, boost::ref(*this));
	stateManager.states.push_back(pThisState);

	// Add a "both sets" phase.
	pThisState = new State(boost::bind(&GameModule::render_frame_game, boost::ref(*this)),
		boost::bind(&GameModule::handleInputGame, boost::ref(*this), _1));
	pThisState->initializeFn = boost::bind(&GameModule::initializeBothSets, boost::ref(*this));
	stateManager.states.push_back(pThisState);

	// Add a "both sets reverse color" phase.
	pThisState = new State(boost::bind(&GameModule::render_frame_game, boost::ref(*this)),
		boost::bind(&GameModule::handleInputGame, boost::ref(*this), _1));
	pThisState->initializeFn = boost::bind(&GameModule::initializeBothSetsReverseColor, boost::ref(*this));
	stateManager.states.push_back(pThisState);

	// Add a "both sets low contrast reverse color" phase.
	pThisState = new State(boost::bind(&GameModule::render_frame_game, boost::ref(*this)),
		boost::bind(&GameModule::handleInputGame, boost::ref(*this), _1));
	pThisState->initializeFn = boost::bind(&GameModule::initializeBothSetsLowContrastReverseColor, boost::ref(*this));
	stateManager.states.push_back(pThisState);

	// Add a "both sets low contrast" phase.
	pThisState = new State(boost::bind(&GameModule::render_frame_game, boost::ref(*this)),
		boost::bind(&GameModule::handleInputGame, boost::ref(*this), _1));
	pThisState->initializeFn = boost::bind(&GameModule::initializeBothSetsLowContrast, boost::ref(*this));
	stateManager.states.push_back(pThisState);

	// Add a "both sets flash" phase.
	pThisState = new State(boost::bind(&GameModule::render_frame_game, boost::ref(*this)),
		boost::bind(&GameModule::handleInputGame, boost::ref(*this), _1));
	pThisState->initializeFn = boost::bind(&GameModule::initializeBothSetsFlash, boost::ref(*this));
	stateManager.states.push_back(pThisState);

	// Add a "both sets same color" phase.
	pThisState = new State(boost::bind(&GameModule::render_frame_game, boost::ref(*this)),
		boost::bind(&GameModule::handleInputGame, boost::ref(*this), _1));
	pThisState->initializeFn = boost::bind(&GameModule::initializeBothSetsSameColor, boost::ref(*this));
	stateManager.states.push_back(pThisState);

	// Add a "first set" phase.
	pThisState = new State(boost::bind(&GameModule::render_frame_game, boost::ref(*this)),
		boost::bind(&GameModule::handleInputGame, boost::ref(*this), _1));
	pThisState->initializeFn = boost::bind(&GameModule::initializeFirstSet, boost::ref(*this));
	stateManager.states.push_back(pThisState);

	// Add a "both sets same color" phase.
	pThisState = new State(boost::bind(&GameModule::render_frame_game, boost::ref(*this)),
		boost::bind(&GameModule::handleInputGame, boost::ref(*this), _1));
	pThisState->initializeFn = boost::bind(&GameModule::initializeBothSetsSameColor, boost::ref(*this));
	stateManager.states.push_back(pThisState);

	// Add a "second set" phase.
	pThisState = new State(boost::bind(&GameModule::render_frame_game, boost::ref(*this)),
		boost::bind(&GameModule::handleInputGame, boost::ref(*this), _1));
	pThisState->initializeFn = boost::bind(&GameModule::initializeSecondSet, boost::ref(*this));
	stateManager.states.push_back(pThisState);

	// Add a "both sets same color" phase.
	pThisState = new State(boost::bind(&GameModule::render_frame_game, boost::ref(*this)),
		boost::bind(&GameModule::handleInputGame, boost::ref(*this), _1));
	pThisState->initializeFn = boost::bind(&GameModule::initializeBothSetsSameColor, boost::ref(*this));
	// ADD "write data" cleanup fn to last one.
	pThisState->cleanupFn = boost::bind(&GameModule::writeData, boost::ref(*this));
	stateManager.states.push_back(pThisState);
}


void GameModule::setUpPhases()
{
	// Instructions preliminaries.
	string instrHeader = "========== VIDEO GAME ==========\n\n";
	string instrFooter = "\n\n\nPress any key to continue >>>>>";
	string instrA;
	string instrB;
	Qubit* pThisQubit;
	State* pThisState;

	instrA = instrHeader + ""
		"As before, there will be two sets of falling words, and your task is to move\n"
		"the platform so that it touches one set of words and avoids touching the other\n"
		"set of words.  The words are the same as before:\n\n"
		"You should TOUCH these words:  " + Util::getInstance().firstGameWords[0] + " and " + Util::getInstance().firstGameWords[1] + "\n\n"
		"You should AVOID these words:  " + Util::getInstance().secondGameWords[0] + " and " + Util::getInstance().secondGameWords[1] + "\n\n"
		"The colors of the words will change periodically -- they may reverse, \n"
		"become harder to distinguish, or both sets may be the same color./\n\n"
		"Regardless of the colors, your task is always the same -- to approach/touch one set\n"
		"of words and avoid touching the other set.  The words will remain the same.\n\n"
		"Again, the words to approach / avoid will be shown at the top of the screen to help\n"
		"you remember."
		"" + instrFooter;
	instrB = instrHeader + ""
		"As before, there will be two sets of falling words, and your task is to move the\n"
		"platform so that it it pops one set of words by touching them and allows the other\n"
		"set of words to reach the ground by not touching them.  The words are the same as before:\n\n"
		"You should POP these words:  " + Util::getInstance().firstGameWords[0] + " and " + Util::getInstance().firstGameWords[1] + "\n\n"
		"You should ALLOW these words:  " + Util::getInstance().secondGameWords[0] + " and " + Util::getInstance().secondGameWords[1] + "\n\n"
		"The colors of the words will change periodically -- they may reverse, become harder to\n"
		"distinguish, or both sets may be the same color.\n\n"  
		"Regardless of the colors, your task is always the same -- to pop/touch one set of words\n"
		"to prevent them from reaching the ground, and to allow the other set to reach the ground\n"
		"by not touching them. The words will remain the same.\n\n"
		"Again, the words to approach / avoid will be shown at the top of the screen to help\n"
		"you remember."
		"" + instrFooter;
	pThisQubit = new DisplayUntilKeypressQubit(Util::getInstance().randomization_game_instructions ? instrA : instrB, 34, true);
	qubits.push_back(pThisQubit);
	pThisState = new State(*pThisQubit);
	pThisState->cleanupFn = boost::bind(&GameModule::resetState, boost::ref(*this));
	stateManager.states.push_back(pThisState);

	// Add a "both sets low contrast" phase.
	pThisState = new State(boost::bind(&GameModule::render_frame_game, boost::ref(*this)),
		boost::bind(&GameModule::handleInputGame, boost::ref(*this), _1));
	pThisState->initializeFn = boost::bind(&GameModule::initializeBothSetsLowContrast, boost::ref(*this));
	stateManager.states.push_back(pThisState);

	// Add a "both sets flash" phase.
	pThisState = new State(boost::bind(&GameModule::render_frame_game, boost::ref(*this)),
		boost::bind(&GameModule::handleInputGame, boost::ref(*this), _1));
	pThisState->initializeFn = boost::bind(&GameModule::initializeBothSetsFlash, boost::ref(*this));
	stateManager.states.push_back(pThisState);

	// Add a "both sets same color" phase.
	pThisState = new State(boost::bind(&GameModule::render_frame_game, boost::ref(*this)),
		boost::bind(&GameModule::handleInputGame, boost::ref(*this), _1));
	pThisState->initializeFn = boost::bind(&GameModule::initializeBothSetsSameColor, boost::ref(*this));
	stateManager.states.push_back(pThisState);

	// Add a "both sets same color" phase.
	pThisState = new State(boost::bind(&GameModule::render_frame_game, boost::ref(*this)),
		boost::bind(&GameModule::handleInputGame, boost::ref(*this), _1));
	pThisState->initializeFn = boost::bind(&GameModule::initializeBothSetsSameColor, boost::ref(*this));
	stateManager.states.push_back(pThisState);

	// Add a "first set" phase.
	pThisState = new State(boost::bind(&GameModule::render_frame_game, boost::ref(*this)),
		boost::bind(&GameModule::handleInputGame, boost::ref(*this), _1));
	pThisState->initializeFn = boost::bind(&GameModule::initializeFirstSet, boost::ref(*this));
	stateManager.states.push_back(pThisState);

	// Add a "both sets same color" phase.
	pThisState = new State(boost::bind(&GameModule::render_frame_game, boost::ref(*this)),
		boost::bind(&GameModule::handleInputGame, boost::ref(*this), _1));
	pThisState->initializeFn = boost::bind(&GameModule::initializeBothSetsSameColor, boost::ref(*this));
	stateManager.states.push_back(pThisState);

	// Add a "both sets same color" phase.
	pThisState = new State(boost::bind(&GameModule::render_frame_game, boost::ref(*this)),
		boost::bind(&GameModule::handleInputGame, boost::ref(*this), _1));
	pThisState->initializeFn = boost::bind(&GameModule::initializeBothSetsSameColor, boost::ref(*this));
	stateManager.states.push_back(pThisState);

	// Add a "second set" phase.
	pThisState = new State(boost::bind(&GameModule::render_frame_game, boost::ref(*this)),
		boost::bind(&GameModule::handleInputGame, boost::ref(*this), _1));
	pThisState->initializeFn = boost::bind(&GameModule::initializeSecondSet, boost::ref(*this));
	stateManager.states.push_back(pThisState);

	// Add a "both sets same color" phase.
	pThisState = new State(boost::bind(&GameModule::render_frame_game, boost::ref(*this)),
		boost::bind(&GameModule::handleInputGame, boost::ref(*this), _1));
	pThisState->initializeFn = boost::bind(&GameModule::initializeBothSetsSameColor, boost::ref(*this));
	stateManager.states.push_back(pThisState);

	// Add a "both sets same color" phase.
	pThisState = new State(boost::bind(&GameModule::render_frame_game, boost::ref(*this)),
		boost::bind(&GameModule::handleInputGame, boost::ref(*this), _1));
	pThisState->initializeFn = boost::bind(&GameModule::initializeBothSetsSameColor, boost::ref(*this));
	// ADD "write data" cleanup fn to last one.
	pThisState->cleanupFn = boost::bind(&GameModule::writeData, boost::ref(*this));
	stateManager.states.push_back(pThisState);
}


void GameModule::initializeFirstSet()
{
	whichGameWordsSet = GAME_WORDS_SET_FIRST;
	setWordAttributes(firstColorSet1, firstColorSet1, false, secondColorSet1, secondColorSet1, false);
	timer.start();
}


void GameModule::initializeSecondSet()
{
	whichGameWordsSet = GAME_WORDS_SET_SECOND;
	setWordAttributes(firstColorSet1, firstColorSet1, false, secondColorSet1, secondColorSet1, false);
	timer.start();
}


void GameModule::initializeBothSets()
{
	whichGameWordsSet = GAME_WORDS_SET_BOTH;
	setWordAttributes(firstColorSet1, firstColorSet1, false, secondColorSet1, secondColorSet1, false);
	timer.start();
}


void GameModule::initializeBothSetsReverseColor()
{
	whichGameWordsSet = GAME_WORDS_SET_BOTH;
	setWordAttributes(secondColorSet1, secondColorSet1, false, firstColorSet1, firstColorSet1, false);
	timer.start();
}


void GameModule::initializeBothSetsLowContrast()
{
	whichGameWordsSet = GAME_WORDS_SET_BOTH;
	setWordAttributes(firstColorSet2, firstColorSet2, false, secondColorSet2, secondColorSet2, false);
	timer.start();
}


void GameModule::initializeBothSetsLowContrastReverseColor()
{
	whichGameWordsSet = GAME_WORDS_SET_BOTH;
	setWordAttributes(secondColorSet2, secondColorSet2, false, firstColorSet2, firstColorSet2, false);
	timer.start();
}


void GameModule::initializeBothSetsFlash()
{
	whichGameWordsSet = GAME_WORDS_SET_BOTH;
	setWordAttributes(firstColorSet2, firstColorSet3, true, secondColorSet2, secondColorSet3, true);
	timer.start();
}


void GameModule::initializeBothSetsSameColor()
{
	whichGameWordsSet = GAME_WORDS_SET_BOTH;
	setWordAttributes(firstColorSet3, firstColorSet3, false, secondColorSet3, secondColorSet3, false);
	timer.start();
}


void GameModule::setWordAttributes(Color firstSetCurrentColor, Color firstSetFlashColor, bool firstSetFlash, 
	Color secondSetCurrentColor, Color secondSetFlashColor, bool secondSetFlash)
{
	firstColor = firstSetCurrentColor;
	firstColorFlash = firstSetFlashColor;
	firstFlash = firstSetFlash;
	secondColor = secondSetCurrentColor;
	secondColorFlash = secondSetFlashColor;
	secondFlash = secondSetFlash;
}

	
int GameModule::render_frame()
{
	int retVal = stateManager.executeStep();
	if (retVal == StateManager::STATE_MANAGER_FINISHED) return Util::MODULE_STATE_FINISHED;
	else return Util::MODULE_STATE_RUNNING;
}


int GameModule::render_frame_game()
{
	// Set up a new frame.
	Graphics::getInstance().setUpNewFrame();

	// Update our state.
	updateState();

	// Potentially add new words.
	addNewWords();

	// Draw the words.
	drawWords();

	// Draw top bar.
	drawTopBar();
	
	// Draw the avatar.
	drawAvatar();

	// Finish and present the frame.
	Graphics::getInstance().finishAndPresentFrame();

	// Return the appropriate state, depending on if we have reached our time limit.
	long elapsedMillis = timer.elapsedTimeMillis();
	if (elapsedMillis >= phaseDurationMillis)
	{
		return State::STATE_TRANSITION_NEXT;
	}
	else return State::STATE_TRANSITION_KEEP_CURRENT;
}


// Update our state.
void GameModule::updateState()
{
	// Update avatar position.
	if (leftArrow) avatarX += avatarVelocityLeft; else if (rightArrow) avatarX += avatarVelocityRight;
	avatarX = min((float) avatarMaxX, max((float) avatarMinX, avatarX));

	// Update word positions.
	float droppingVelocity = getProportionValue(droppingVelocityProportion, droppingVelocityMaxEasy, droppingVelocityMaxHard);
	for (auto thisWord = currentWords.begin();  thisWord != currentWords.end();  ++thisWord)
	{
		// Update position.
		(*thisWord).positionY += (int) droppingVelocity;
	}

	// Update flash timer.
	long flashElapsedMillis = flashTimer.elapsedTimeMillis();
	if (flashElapsedMillis > 2000)
	{
		// Toggle flash on.
		flashIsOn = !flashIsOn;

		// Reset flash timer.
		flashTimer.start();
	}

	// Update game word states.
	if (false)
	{
		bool slowItDown = false;
		bool addNewGameWords = false;
		if (false && addNewGameWords && firstGameWordsSet.size() < Util::getInstance().firstGameWords.size())
		{
			// Get a new "first set" word not already contained in our list.
			vector<string> currentWordsFirst;
			for (auto iter = firstGameWordsSet.begin(); iter != firstGameWordsSet.end(); ++iter)
			{
				currentWordsFirst.push_back(iter->second.wordText);
			}
			vector<string> newWordVectorFirst = 
				Util::getInstance().randomElementsNotIn(Util::getInstance().firstGameWords, 1, currentWordsFirst, stringEquals);
			string newWordFirst = newWordVectorFirst[0];
			GameWord thisGameWordFirst(newWordFirst);
			firstGameWordsSet[newWordFirst] = thisGameWordFirst;

			// Get a new "second set" word not already contained in our list.
			vector<string> currentWordsSecond;
			for (auto iter = secondGameWordsSet.begin(); iter != secondGameWordsSet.end(); ++iter)
			{
				currentWordsSecond.push_back(iter->second.wordText);
			}
			vector<string> newWordVectorSecond = 
				Util::getInstance().randomElementsNotIn(Util::getInstance().secondGameWords, 1, currentWordsSecond, stringEquals);
			string newWordSecond = newWordVectorSecond[0];
			GameWord thisGameWordSecond(newWordSecond);
			secondGameWordsSet[newWordSecond] = thisGameWordSecond;
		}
		if (slowItDown)
		{
			droppingVelocityProportion = min(1.0f, max(0.0f, droppingVelocityProportion - 0.2f));
			framesBetweenWordsProportion = droppingVelocityProportion;
			displayRadiusProportion = droppingVelocityProportion;
		}
	}

	// Process words that are about to go off the screen.
	for (auto iter = currentWords.begin(); iter != currentWords.end(); ++iter)
	{
		// Word is about to go off the screen, let's see what happened.
		if ((*iter).positionY > Graphics::getInstance().displayHeight)
		{
			float deltaProportion = 0.0f;
			if ((*iter).wasTouchedByAvatar && (*iter).shouldAvoid) deltaProportion = -0.02f;
			else if ((*iter).wasTouchedByAvatar && !(*iter).shouldAvoid) deltaProportion = 0.01f;
			else if (!(*iter).wasTouchedByAvatar && (*iter).shouldAvoid) deltaProportion = 0.01f;
			else if (!(*iter).wasTouchedByAvatar && !(*iter).shouldAvoid) deltaProportion = -0.02f;
			droppingVelocityProportion = min(1.0f, max(0.0f, droppingVelocityProportion + deltaProportion));
			framesBetweenWordsProportion = droppingVelocityProportion;
			displayRadiusProportion = droppingVelocityProportion;
		}
	}

	// Remove words that have gone off the screen.
	struct isOffTheScreen {
		bool operator() (const DroppingWord& value) 
		{ 
			return value.positionY > Graphics::getInstance().displayHeight; 
		}
	};
	currentWords.remove_if(isOffTheScreen());

	// Update number of frames since we added a new word.
	framesSinceNewWord++;
}


void GameModule::resetState()
{
	// Set keyboard state.
	leftArrow = false;
	rightArrow = false;

	// Which word set we used last, and how many repeats of the same set.
	lastWordSet = Util::getInstance().randomInt(0, 2) == 0 ? true : false;
	wordSetRepeatNum = 0;

	// Initial value for number of frames since last new word started dropping.
	framesSinceNewWord = 1000;

	// Set initial avatar position.
	avatarX = (Graphics::getInstance().displayWidth - avatarWidth) / 2.0f;
	avatarY = (float) Graphics::getInstance().displayHeight - avatarHeight;

	// List of words currently on the screen.
	currentWords.clear();

	// Our timers for the current dropping words game.
	timer.start();
	flashTimer.start();
}


// Add new words (potentially).
void GameModule::addNewWords()
{
	// Determine whether to add the new word.
	int framesBetweenWords = (int) floor(0.5f + getProportionValue(framesBetweenWordsProportion,
		(float) framesBetweenWordsMaxEasy, (float) framesBetweenWordsMaxHard));
	bool addNewWord = framesSinceNewWord > framesBetweenWords ? true : false;
	if (!addNewWord) return;

	// Determine which word set to use.
	bool useFirstSet;
	if (whichGameWordsSet == GAME_WORDS_SET_BOTH)
	{
		useFirstSet = Util::getInstance().randomInt(0, 2) == 0 ? true : false;
		if (lastWordSet && wordSetRepeatNum >= 3) useFirstSet = false;
		if (!lastWordSet && wordSetRepeatNum >= 3) useFirstSet = true;
	}
	else if (whichGameWordsSet == GAME_WORDS_SET_FIRST)
	{
		useFirstSet = true;
	}
	else  // whichGameWordsSet == GAME_WORDS_SET_SECOND
	{
		useFirstSet = false;
	}

	// Select the new word.
	GameWord newGameWord;
	if (useFirstSet)
	{
		// Select a random word.
		auto iter = firstGameWordsSet.begin();
		advance(iter, Util::getInstance().randomInt(0, firstGameWordsSet.size()));
		newGameWord = iter->second;
	}
	else
	{
		// Select a random word.
		auto iter = secondGameWordsSet.begin();
		advance(iter, Util::getInstance().randomInt(0, secondGameWordsSet.size()));
		newGameWord = iter->second;
	}

	// Determine screen width and height for new word.
	RECT textBox = Graphics::getInstance().measureString(Graphics::getInstance().wordFont, newGameWord.wordText);
	int screenWidth = textBox.right - textBox.left;
	int screenHeight = textBox.bottom - textBox.top;

	// Determine x position for new word.
	float displayRadius = getProportionValue(displayRadiusProportion, displayRadiusMaxEasy, displayRadiusMaxHard);
	float currentXProportion = (float) lastWord.positionX / (Graphics::getInstance().displayWidth - screenWidth);
	int proportionDeltaXLower = (int) floor(-displayRadius * 100.0f + 0.5f);
	int proportionDeltaXUpper = (int) floor(displayRadius * 100.0f + 1.5f);
	float proportionDeltaX = Util::getInstance().randomInt(proportionDeltaXLower, proportionDeltaXUpper) * 0.01f;
	float newXProportion = currentXProportion + proportionDeltaX;
	if (newXProportion < 0.0f || newXProportion > 1.0f) newXProportion = newXProportion - 1.5f * proportionDeltaX;
	int positionX = (int) (newXProportion * (Graphics::getInstance().displayWidth - screenWidth));
	positionX = max(0, positionX);
	positionX = min(positionX, Graphics::getInstance().displayWidth - screenWidth);

	// Add the new word.
	if (addNewWord)
	{
		DroppingWord newWord;
		newWord.wordText = newGameWord.wordText;
		if (useFirstSet)
		{
			newWord.shouldAvoid = false;
			if (lastWordSet) wordSetRepeatNum++;
			else wordSetRepeatNum = 1;
			lastWordSet = true;
		}
		else
		{
			newWord.shouldAvoid = true;
			if (!lastWordSet) wordSetRepeatNum++;
			else wordSetRepeatNum = 1;
			lastWordSet = false;
		}
		RECT textBox = Graphics::getInstance().measureString(Graphics::getInstance().wordFont, newWord.wordText);
		newWord.screenWidth = screenWidth;
		newWord.screenHeight = screenHeight;
		newWord.positionY = - newWord.screenHeight;
		newWord.positionX = positionX;
		newWord.isFromSetA = useFirstSet;
		currentWords.push_back(newWord);
		framesSinceNewWord = 0; 
		lastLastWord = lastWord;
		lastWord = newWord;
	}
}


// Draw words to the screen.
void GameModule::drawWords()
{
	for (auto thisWord = currentWords.begin(); thisWord != currentWords.end(); ++thisWord)
	{
		Color wordColor;
		if (thisWord->isFromSetA)
		{
			if (firstFlash && flashIsOn) wordColor = firstColorFlash;
			else wordColor = firstColor;
		}
		else
		{
			if (secondFlash && flashIsOn) wordColor = secondColorFlash;
			else wordColor = secondColor;
		}

		// Draw the word.
		Graphics::getInstance().drawText(thisWord->wordText, thisWord->positionX, thisWord->positionY,
			Graphics::getInstance().wordFont, wordColor);
	}
}


// Draw the avatar to the screen.
void GameModule::drawAvatar()
{
	// Determine avatar location rectangle.
	static RECT rect;
	rect.left = (long) avatarX;
	rect.right = (long) (avatarX + avatarWidth - 1);
	rect.top = Graphics::getInstance().displayHeight - avatarHeight + 1;
	rect.bottom = Graphics::getInstance().displayHeight;

	// Determine avatar color.
	static Color color;
	if (avatarTouchingWords()) color = Color(255, 255, 255);
	else color = Color(200, 200, 200);

	// Draw the avatar.
	Graphics::getInstance().drawRectangle(rect, color);
}


// Draw the top bar.
void GameModule::drawTopBar()
{
	// Draw the top bar background.
	static RECT bgRect;
	bgRect.left = 0;
	bgRect.right = Graphics::getInstance().displayWidth;
	bgRect.top = 0;
	bgRect.bottom = Graphics::getInstance().displayHeight / 15;
	Color bgColor = Color(0, 0, 0);
	Graphics::getInstance().drawRectangle(bgRect, bgColor);

	// Draw the top bar line.
	static RECT lineRect;
	lineRect.left = 0;
	lineRect.top = Graphics::getInstance().displayHeight / 15;
	lineRect.right = Graphics::getInstance().displayWidth;
	lineRect.bottom = Graphics::getInstance().displayHeight / 15;
	static Color lineColor = Color(255, 255, 255);
	Graphics::getInstance().drawLine(lineRect, lineColor);

	// Draw a horizontal middle dividing line.
	static RECT midRect;
	midRect.left = Graphics::getInstance().displayWidth / 2;
	midRect.top = 0;
	midRect.right = Graphics::getInstance().displayWidth / 2;
	midRect.bottom = Graphics::getInstance().displayHeight / 15;
	Graphics::getInstance().drawLine(midRect, lineColor);

	// Initialize top bar positioning variables.
	static bool initBarText = true;
	static int posY;
	static int spacerX;
	if (initBarText)
	{
		spacerX = Graphics::getInstance().displayWidth / 40;
		RECT wordRECT = Graphics::getInstance().measureString(Graphics::getInstance().standardFont, "Hello");
		int wordHeight = wordRECT.bottom - wordRECT.top;
		posY = ((Graphics::getInstance().displayHeight / 15) - wordHeight) / 2;
	}

	// Display the left set of words.
	int currentX = spacerX;
	RECT thisRECT;
	string thisLabel;
	string thisText;
	Color thisColor;
	if (whichGameWordsSet == GAME_WORDS_SET_FIRST || whichGameWordsSet == GAME_WORDS_SET_BOTH)
	{
		thisLabel = Util::getInstance().randomization_game_instructions ? "APPROACH:" : "POP:";
		thisRECT = Graphics::getInstance().measureString(Graphics::getInstance().standardFont, thisLabel);
		Graphics::getInstance().drawText(thisLabel, currentX, posY, Graphics::getInstance().standardFont, lineColor);
		currentX += (thisRECT.right - thisRECT.left + spacerX);
		thisColor = firstFlash && flashIsOn ? firstColorFlash : firstColor;
		for (auto iter = Util::getInstance().firstGameWords.begin(); iter != Util::getInstance().firstGameWords.end(); ++iter)
		{
			thisText = *iter;
			thisRECT = Graphics::getInstance().measureString(Graphics::getInstance().standardFont, thisText);
			Graphics::getInstance().drawText(thisText, currentX, posY, Graphics::getInstance().standardFont, thisColor);
			currentX += (thisRECT.right - thisRECT.left + spacerX);
		}
	}

	// Display the right set of words.
	if (whichGameWordsSet == GAME_WORDS_SET_SECOND || whichGameWordsSet == GAME_WORDS_SET_BOTH)
	{
		currentX = Graphics::getInstance().displayWidth / 2 + spacerX;
		thisLabel = Util::getInstance().randomization_game_instructions ? "AVOID:" : "ALLOW:";
		thisRECT = Graphics::getInstance().measureString(Graphics::getInstance().standardFont, thisLabel);
		Graphics::getInstance().drawText(thisLabel, currentX, posY, Graphics::getInstance().standardFont, lineColor);
		currentX += (thisRECT.right - thisRECT.left + spacerX);
		thisColor = secondFlash && flashIsOn ? secondColorFlash : secondColor;
		for (auto iter = Util::getInstance().secondGameWords.begin(); iter != Util::getInstance().secondGameWords.end(); ++iter)
		{
			thisText = *iter;
			thisRECT = Graphics::getInstance().measureString(Graphics::getInstance().standardFont, thisText);
			Graphics::getInstance().drawText(*iter, currentX, posY, Graphics::getInstance().standardFont, thisColor);
			currentX += (thisRECT.right - thisRECT.left + spacerX);
		}
	}
}


// Write data.
void GameModule::writeData()
{
	Util::getInstance().outputSS << "<game>" << endl;
	Util::getInstance().outputSS << "</game>" << endl;
}


// Destructor.
GameModule::~GameModule()
{
	for (auto iter = qubits.begin(); iter != qubits.end(); ++iter)
	{
		delete (*iter);
	}

	for (auto iter = stateManager.states.begin(); iter != stateManager.states.end(); ++iter)
	{
		delete (*iter);
	}
}


// Handle keyboard input.
LRESULT GameModule::handleInput(RAWINPUT InputData)
{
	return stateManager.handleInput(InputData);
}


LRESULT GameModule::handleInputGame(RAWINPUT InputData)
{
	USHORT vKey = InputData.data.keyboard.VKey;
	bool isDown = (InputData.data.keyboard.Message == WM_KEYDOWN ? true : false);
	bool isUp = (InputData.data.keyboard.Message == WM_KEYUP ? true : false);

	if (vKey == VK_LEFT && isDown) { leftArrow = true; }
	else if (vKey == VK_LEFT && isUp) { leftArrow = false; }
	else if (vKey == VK_RIGHT && isDown) { rightArrow = true; }
	else if (vKey == VK_RIGHT && isUp) { rightArrow = false; }

	return 0;
}


bool GameModule::rectanglesOverlap(int x1, int y1, int xwidth1, int ywidth1, int x2, int y2, int xwidth2, int ywidth2)
{
	bool xOverlaps = false;
	if ((x1 == x2) || (x1 < x2 && (x1 + xwidth1 - 1) >= x2) || (x2 < x1 && (x2 + xwidth2 - 1) >= x1))
	{
		xOverlaps = true;
	}

	bool yOverlaps = false;
	if ((y1 == y2) || (y1 < y2 && (y1 + ywidth1 - 1) >= y2) || (y2 < y1 && (y2 + ywidth2 - 1) >= y1))
	{
		yOverlaps = true;
	}

	return xOverlaps && yOverlaps;
}


bool GameModule::avatarTouchingWords()
{
	for (auto thisWord = currentWords.begin();  thisWord != currentWords.end();  ++thisWord)
	{
		if (rectanglesOverlap((int) avatarX, (int) avatarY, avatarWidth, avatarHeight, (*thisWord).positionX,
			(*thisWord).positionY, (*thisWord).screenWidth, (*thisWord).screenHeight))
		{
			(*thisWord).wasTouchedByAvatar = true;
			return true;
		}
	}

	return false;
}


float GameModule::getProportionValue(float proportion, float minValue, float maxValue)
{
	return minValue + proportion * (maxValue - minValue);
}


int GameModule::progressTimeSeconds()
{
	return 240;
}


// String equals.
bool stringEquals(string& one, string& two)
{
	return one.compare(two) == 0;
}




