#include "stdafx.h"

#include "WordSet.h"
#include "Color.h"


WordSet::WordSet()
{
	words.clear();
	wordColor = Color(255, 255, 255, 255);
}


WordSet::WordSet(const vector<string>& words, Color wordColor)
{
	this->words.assign(words.begin(), words.end());
	this->wordColor = wordColor;
}


