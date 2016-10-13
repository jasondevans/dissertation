#pragma once

#include "stdafx.h"
#include "Timer.h"


using namespace std;


class Util
{

public:

	// Qubit running/finished states.
	static const int QUBIT_STATE_RUNNING = 1;
	static const int QUBIT_STATE_FINISHED = 2;

	// Module running/finished states.
	static const int MODULE_STATE_RUNNING = 1;
	static const int MODULE_STATE_FINISHED = 2;

	// Experiment running/finished states.
	static const int EXPERIMENT_STATE_RUNNING = 1;
	static const int EXPERIMENT_STATE_FINISHED = 2;

	// Whether we're running in full-screen mode.
	bool fullScreen;

	// Whether we're replicating Draine and Greenwald, instead of running the actual experiment.
	bool replicateDG;

	// Whether to upload our results.
	bool uploadResults;

	// Whether to encrypt data files.
	bool encryptDataFiles;

	// First set of game words.
	vector<string> firstGameWords;

    // Second set of game words.
	vector<string> secondGameWords;

	// First practice word set.
	vector<string> gamePracticeWords;

	// Lead-in words for the priming task.
	vector<string> leadInWords;

    // Consonants.
	vector<string> consonants;

    // Pleasant words.
	vector<string> pleasantWords;

    // Unpleasant words.
	vector<string> unpleasantWords;

    // The length of our string mask.
	int maskLength;

	// A random number generator, Mersenne Twister.
	boost::mt19937 randomGen;

	// The current subject id.
	int subjectId;

	// The fully-qualified directory name of our data file.
	wstring dataFileDirectory;

	// The filename of our data file.
	wstring dataFileName;

	// The filename of our consent file.
	wstring consentFileName;

	// The filename of our test file.
	wstring testFileName;

	// Output file stream.
	ofstream outputFile;

	// Consent file stream.
	ofstream consentFile;

	// Test file stream.
	ofstream testFile;

	// Output string stream.
	stringstream outputSS;

	// Consent string stream.
	stringstream consentSS;

	// Randomization: Which game instruction set to use:
	// true = straightforward approach / avoid
	// false = touching is avoidance
	bool randomization_game_instructions;

	// Randomization: Which word set is tied to spatial approach in the game:
	// true = first set is spatial approach, second set is spatial avoidance
	// false = first set is spatial avoidance, second set is spatial approach.
	bool randomization_game_wordset;

	// Randomization: Which key is mapped to positive / negative response in
	// the priming task:
	// true = left key is positive, right key is negative
	// false = left key is negative, right key is positive
	bool randomization_priming_keymap;

	// Number of frames before the response window.
	int framesBeforeResponseWindow;

	// Priming response timer.
	Timer primingResponseTimer;

	// Total experiment time in seconds, for purposed of progress indicators (estimated).
	int totalProgressTime;

	// Elapsed experiment time in seconds, for purposes of progress indicators (estimated).
	int elapsedProgressTime;

	// Whether to use priming timing (within display qubits, used in priming module).
	bool primingTimingEnabled;

	// Whether priming timing responses are allowed.
	bool primingTimingCanRespond;

	// Whether priming timing response has occurred.
	bool primingTimingHasResponded;

	// Priming timing countdown number of frames.
	int primingTimingCountdownFrames;
	

	// Constructor.
	Util();
	
	// Return a reference to our singleton.
	static Util& getInstance()
    {
        static Util instance;
        return instance;
    }

	// Destructor.
	~Util();

	// Generate a random integer with the specified bounds.
	int randomInt(int lowerInclusive, int upperExclusive);

	// Randomly sort a list, using Fisher-Yates.
	template<typename T>
	void randomSort(vector<T>& theList)
	{
		for (int i = theList.size() - 1; i >= 1; i--)
		{
			int j = randomInt(0, i + 1);
			T temp(theList[i]);
			theList[i] = theList[j];
			theList[j] = temp;
		}
	}  

	// Return a number of unique-by-index random elements from a vector.
	template<typename T>
	vector<T> randomElementsUniqueByIndex(vector<T>& theList, int numElements)
	{
		vector<T> resultVector;
		vector<int> usedIndexes;
		for (int i = 0; i < numElements; i++)
		{
			int thisIndex = randomInt(0, theList.size());
			while (std::find(usedIndexes.begin(), usedIndexes.end(), thisIndex) != usedIndexes.end())
			{
				thisIndex = randomInt(0, theList.size());
			}
			usedIndexes.push_back(thisIndex);
			resultVector.push_back(theList[thisIndex]);
		}
		return resultVector;
	}

	// Return a number of random elements from a vector, elements are unique by index
	// unless numElements is greater than the size of the vector, in which case
	// additional (repeat) elements are selected up to 2x the size of the vector, and 
	// so on.
	template<typename T>
	vector<T> randomElements(vector<T>& theList, int numElements)
	{
		vector<T> resultVector;
		int numAdded = 0;
		while (numAdded < numElements)
		{
			int numElementsThisIter = min((int) theList.size(), numElements - numAdded);
			vector<T> theseElements = randomElementsUniqueByIndex(theList, numElementsThisIter);
			resultVector.insert(resultVector.end(), theseElements.begin(), theseElements.end());
			numAdded += numElementsThisIter;
		}
		return resultVector;
	}

	// Return a number of random elements from a vector, where none of the elements 
	// returned are in a second vector (as determined by a passed in comparison function).
	template<typename T>
	vector<T> randomElementsNotIn(vector<T>& theList, int numElements, vector<T>& notList, bool (*equals)(T&, T&))
	{
		// Create a copy of the list, removing any elements that are in the not-list.
		vector<T> theCleanList;
		for (auto iter = theList.begin(); iter != theList.end(); ++iter)
		{
			if (!contains(notList, *iter, equals)) theCleanList.push_back(*iter);
		}

		// Return numElements from the clean list.
		return randomElements(theCleanList, numElements);
	}

	// Return a randomly sorted list combined from an equal number of random elements
	// from two lists.  If number is odd, one list will contribute one more element 
	// than the other list.
	template<typename T>
	vector<T> randomList(vector<T>& list1, vector<T>& list2, int numTotalElements)
	{
		vector<T> resultVector;
		vector<T> randomList1 = randomElements(list1, numTotalElements / 2);
		vector<T> randomList2 = randomElements(list2, numTotalElements / 2);
		vector<T> extraElementV = randomInt(0, 2) == 0 ? randomElements(randomList1, 1) : randomElements(randomList2, 1);
		if (numTotalElements % 2 == 1 /* is odd */) resultVector.push_back(extraElementV[0]);
		resultVector.insert(resultVector.end(), randomList1.begin(), randomList1.end());
		resultVector.insert(resultVector.end(), randomList2.begin(), randomList2.end());
		randomSort(resultVector);
		return resultVector;
	}

	// Determine whether the given vector contains a given element, as determined by the
	// passed in "equals" method.
	template<typename T>
	bool contains(vector<T>& theList, T& testElement, bool (*equals)(T&, T&))
	{
		for (auto iter = theList.begin(); iter != theList.end(); ++iter)
		{
			if (equals(*iter, testElement)) return true;
		}
		return false;
	}

	// Create a random consonant string of a given length.  Optionally, specify
	// a string such that the created string shall have no characters matching
	// the given string in the same position.
	string CreateRandomConsonantString(int length, const string& notLikeThis);

	// Insert newline characters, breaking the given string into lines that are
	// at most maxLineLengthChars characters long.
	string insertNewLines(const string& theString, int maxLineLengthChars);

	// Upload a file.
	bool uploadFile(wstring& sourceFile, wstring& destFile);

	// Finish, write, and close data files.
	void finishAndCloseFiles();

	// Round a float to an int.
	static int round(float f);

	// Convert string to wstring.
	static wstring s2ws(const std::string& str);

	// Convert string to wstring.
	static string ws2s(const std::wstring& wstr);

	// Test our ftp connection (and give user a chance to allow through firewall).
	// Returns true if succeeded, false if failed.
	bool testFtpConnection();

};
