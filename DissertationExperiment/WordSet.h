#pragma once

#include "Color.h"

using namespace std;


class WordSet
{

public:

    vector<string> words;
	Color wordColor;

	WordSet();

	WordSet(const vector<string>& words, Color wordColor);

};
