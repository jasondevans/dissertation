#include "stdafx.h"

#include "Util.h"
#include "../FtpClient/FTPClient.h"



Util::Util()
{ 
	// Initialize our random number generator.
	randomGen.seed((unsigned long) time(0));

	// Set randomization of conditions.
	randomization_game_instructions = true; // randomInt(0, 2) == 0 ? true : false; // true = straightforward instructions
	randomization_game_wordset = randomInt(0, 2) == 0 ? true : false;
	randomization_priming_keymap = randomInt(0, 2) == 0 ? true : false;

	// Whether we should run full screen.
	fullScreen = false;

	// Whether we should replicate DG.
	replicateDG = false;

	// Whether we should upload results.
	uploadResults = false;

	// Whether we should encrypt our data files.
	encryptDataFiles = false;

	// Let's just do CHAIR-BASKET and WAGON-STATUE.

	// Game words, set A.
	// string setAGameWordsArr[] = { "CHAIR", "PENCIL", "CHIN", "WAGON", "BASKET", "LAMP" };
	string setAGameWordsArr[] = { "CHAIR", "BASKET" };

	// Game words, set B.
	// string setBGameWordsArr[] = { "FOOT", "STATUE", "FORK", "BUTTER", "CIRCLE", "QUIET" };
	string setBGameWordsArr[] = { "WAGON", "STATUE" };
	
	// Assign game word sets, depending on randomization.
	if (randomization_game_wordset)
	{
		firstGameWords.insert(firstGameWords.end(), &setAGameWordsArr[0], &setAGameWordsArr[0]+2);
		secondGameWords.insert(secondGameWords.end(), &setBGameWordsArr[0], &setBGameWordsArr[0]+2);
	}
	else
	{
		firstGameWords.insert(firstGameWords.end(), &setBGameWordsArr[0], &setBGameWordsArr[0]+2);
		secondGameWords.insert(secondGameWords.end(), &setAGameWordsArr[0], &setAGameWordsArr[0]+2);
	}

	// Game practice words.
	string gamePracticeWordsArr[] = { "BARREL", "ELBOW", "ENGINE", "BOWL", "SPHERE" };
	gamePracticeWords.insert(gamePracticeWords.end(), &gamePracticeWordsArr[0], &gamePracticeWordsArr[0]+5);

	// Consonants.
	string consonantsArr[] = { "B", "C", "D", "F", "G", "H", "J", "K", "L", "M", "N", "P", 
		"Q", "R", "S", "T", "V", "W", "X", "Z" };
	consonants.insert(consonants.end(), &consonantsArr[0], &consonantsArr[0]+20);

	// Pleasant words.
	string pleasantWordsArr[] = { "HONOR", "LUCKY", "DIAMOND", "LOYAL", "FREEDOM", "RAINBOW", "LOVE", 
		"HONEST", "PEACE", "HEAVEN", "PLEASURE", "FAMILY", "DIPLOMA", "KISS", "CHEER", "HEALTH", 
		"FRIEND", "CARESS", "SUNSET", "HAPPY", "MIRACLE", "SUNRISE", "PARADISE", "VACATION", 
		"TREASURE" };
	pleasantWords.insert(pleasantWords.end(), &pleasantWordsArr[0], &pleasantWordsArr[0]+25);

	// Unpleasant words.
	string unpleasantWordsArr[] = { "EVIL", "CANCER", "SICKNESS", "DISASTER", "POVERTY", "VOMIT", 
		"BOMB", "ROTTEN", "ABUSE", "MURDER", "ASSAULT", "SLIME", "DIVORCE", "POISON", "KILL", 
		"DEATH", "HATRED", "SCUM", "ACCIDENT", "JAIL", "STINK", "TORTURE", "CRASH", "FILTH", 
		"POLLUTE" };
	unpleasantWords.insert(unpleasantWords.end(), &unpleasantWordsArr[0], &unpleasantWordsArr[0]+25);

	// The length of our string mask.
	maskLength = 13;

	// Number of frames before the response window.
	framesBeforeResponseWindow = 20;  // Start with 20.

	// Determine our subject id (randomly for now).
	subjectId = randomInt(1, (std::numeric_limits<int>::max)());

	// Determine our data file directory and name.
	DWORD dwRetVal = 0;
	WCHAR lpTempPathBuffer[MAX_PATH];
	dwRetVal = GetTempPath(MAX_PATH,          // length of the buffer
                           lpTempPathBuffer); // buffer for path
	wstring tempPath = lpTempPathBuffer;
	dataFileDirectory = tempPath + L"JasonsDissertation";
	_wmkdir(dataFileDirectory.c_str());
	/*
	time_t currentTime;
	time(&currentTime);
	long int currentTimeL = (long int) currentTime;
	*/
	wstringstream sstm;
	sstm << "ExperimentData" << subjectId /*currentTimeL*/ << ".xml";
	dataFileName = sstm.str();

	// Determine our consent file name.
	wstringstream csstm;
	csstm << "Consent" << randomInt(1, (std::numeric_limits<int>::max)()) << ".xml";
	consentFileName = csstm.str();

	// Determine our test file name.
	wstringstream tsstm;
	tsstm << "TestFile" << randomInt(1, (std::numeric_limits<int>::max)()) << ".xml";
	testFileName = tsstm.str();

	// Initialize priming timing variables.
	primingTimingEnabled = false;
	primingTimingCanRespond = false;
	primingTimingHasResponded = false;
	primingTimingCountdownFrames = 0;
}


// Generate a random integer with the specified bounds.
int Util::randomInt(int lowerInclusive, int upperExclusive)
{
	boost::random::uniform_int_distribution<int> uint_dist(lowerInclusive, upperExclusive - 1);
	return uint_dist(randomGen);
}


// Create a random consonant string of a given length.  Optionally, specify
// a string such that the created string shall have no characters matching
// the given string in the same position.
string Util::CreateRandomConsonantString(int length, const string& notLikeThis)
{
    string theString = "";
    for (int i = 0; i < length; i++)
    {
        // Get a random index.
        int randomIndex = randomInt(0, consonants.size());

        // Get a potential next letter for the string.
        string candidateLetter = consonants[randomIndex];

        // If we have a "not like this" string, make sure candidate letter doesn't match it at
        // the same position.
        if (!notLikeThis.empty())
        {
            while (candidateLetter.at(0) == notLikeThis.at(i))
            {
                randomIndex = randomInt(0, consonants.size());
                candidateLetter = consonants[randomIndex];
            }
        }

        // Add the candidate letter to the string.
        theString += candidateLetter;
    }

    // Return the string.
    return theString;
}


// Insert newline characters, breaking the given string into lines that are
// at most maxLineLengthChars characters long.
string Util::insertNewLines(const string& theString, int maxLineLengthChars)
{
	string newString;

	while (theString.size() - newString.size() > (unsigned int) maxLineLengthChars)
	{
		string thisSubstr = theString.substr(newString.size(), maxLineLengthChars);
		size_t replacePos = thisSubstr.find_last_of(' ');
		thisSubstr.replace(replacePos, 1, "\n");
		newString = newString + thisSubstr.substr(0, replacePos + 1);
	}
	newString = newString + theString.substr(newString.size());

	return newString;
}


// Upload a file.
bool Util::uploadFile(wstring& sourceFile, wstring& destFile)
{
	// Create an ftp client.
	nsFTP::CFTPClient ftpClient;

	// Specify login information.
	nsFTP::CLogonInfo logonInfo(_T("server_name_goes_here"), 21, _T("ftp_username_goes_here"), _T("password_goes_here"));

	// Connect to the server.
	ftpClient.Login(logonInfo);

	// Upload the file.
	bool result = ftpClient.UploadFile(sourceFile, destFile);

	// Disconnect.
	ftpClient.Logout();

	// Return the result.
	return result;
}


// Finish, write, and close files.
void Util::finishAndCloseFiles()
{
	// Complete the data file.
	outputSS << "</experiment-data>";

	// Get string's containing file contents to write.
	string outputStr = outputSS.str();
	string consentStr = consentSS.str();

	// Possibly encrypt data files before writing.
	if (encryptDataFiles)
	{
		// XOR-"encrypt" the files.
		char encryptKey = 'q';
		for (unsigned int i = 0; i < outputStr.size(); i++)
		{
			outputStr[i] ^= encryptKey;
		}
		for (unsigned int i = 0; i < consentStr.size(); i++)
		{
			consentStr[i] ^= encryptKey;
		}
	}

	// Write the data to file.
	outputFile.open(dataFileDirectory + L"\\" + dataFileName);
	consentFile.open(dataFileDirectory + L"\\" + consentFileName);
	outputFile << outputStr;
	consentFile << consentStr;
	outputFile.close();
	consentFile.close();
}


// Round a float to an int.
int Util::round(float f)
{
	return (int) floor(f + 0.5f);
}


// Convert string to wstring.
wstring Util::s2ws(const std::string& str)
{
    typedef std::codecvt_utf8<wchar_t> convert_typeX;
    std::wstring_convert<convert_typeX, wchar_t> converterX;

    return converterX.from_bytes(str);
}

string Util::ws2s(const std::wstring& wstr)
{
    typedef std::codecvt_utf8<wchar_t> convert_typeX;
    std::wstring_convert<convert_typeX, wchar_t> converterX;

    return converterX.to_bytes(wstr);
}



// Destructor.
Util::~Util()
{
}


// Test our ftp connection (and give user a chance to allow through firewall).
// Returns true if succeeded, false if failed.
bool Util::testFtpConnection()
{
	// Test ftp ability (give user a chance to allow this program through the firewall
	// before going full-screen).
	int uploadMsgBoxID = MessageBox(NULL,
		L"Before beginning the experiment, we need to test to make sure we can "
		L"communicate with the server (needed at the end of the experiment to "
		L"upload the results).\n\nYou may be asked if you want to allow this program "
		L"to access the internet (\"go through the firewall\").  If so, please "
		L"click to Allow Access.  Thank you!", L"Test Server Connection", 
		MB_ICONEXCLAMATION | MB_OK);

	// Upload a test file.
	testFile.open(dataFileDirectory + L"\\" + testFileName);
	testFile << "<test-file>This is a test.</test-file>" << endl;
	testFile.close();
	bool uploadTestResult = uploadFile(dataFileDirectory + L"\\" + testFileName, L"/test/" + testFileName);

	// If our test failed, try 12 more times (or until success).
	for (int i = 0; !uploadTestResult && i < 12; i++)
	{
		// Sleep for 5 seconds.
		Sleep(5000);

		// Try again.
		uploadTestResult = uploadFile(dataFileDirectory + L"\\" + testFileName, L"/test/" + testFileName);
	}

	return uploadTestResult;
}






