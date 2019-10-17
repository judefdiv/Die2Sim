/**
 * Author:  		Jude de Villiers
 * Origin:  		E&E Engineering - Stellenbosch University
 * For:					Supertools, Coldflux Project - IARPA
 * Created: 		2019-03-20
 * Modified:
 * license:
 * Description: Useful general functions.
 * File:				genFunc.cpp
 */

#include "die2sim/genFunc.hpp"

bool isStrDig(string wannaDig){
	int i = 0;
	char tempChar;
	while(wannaDig.length() > i){
		tempChar = (char)wannaDig.at(i);
		if(!isdigit(tempChar) || tempChar != '-'){
			return false;
		}
		i++;
	}
	return true;
}

void printLineArrayF(float disArray[], int arrLen){
	for(int i = 0; i < arrLen; i++){
		cout << disArray[i] << setw(5);
	}
	cout << endl;
}

/**
 * Remove starting and trailing whitespaces
 * @param  inVec [The output VECTOR where separated strings are stored]
 * @param  inStr [The input string that needs to be separated by whitespaces]
 */

void stringSplitVec(vector<string> &inVec, string inStr){
	size_t WSpos = 0;												//Whitespace position
	size_t WSposOld = 0;
	size_t StrLen = 0;
	//char cpBuffer[24];
	string tempDis;
	// int k;

	inVec.clear();

	inStr = cleanWhite(inStr);						// remove whitespaces

	WSposOld = 0;
	StrLen = inStr.length();
	int j = 0;
	while(j < LEFmaxParam){											// extracts the data into a string array
		WSpos = inStr.find(" ", WSposOld);
		if(WSpos > StrLen) WSpos = StrLen;

		inVec.push_back(inStr.substr(WSposOld, WSpos - WSposOld));
		WSposOld = WSpos + 1;

		if(WSpos >= StrLen) break;
	}

  inVec.push_back("\0");
}

/**
 * SplitStrVec - sperates the string by white spaces
 * @param  inStr [The input string that needs to be separated by whitespaces]
 */

vector<string> SplitStrVec(string inStr){
	size_t WSpos = 0;													//Whitespace position
	size_t WSposOld = 0;
	size_t StrLen = 0;
	string tempDis;

	vector<string> fooVec;

	inStr = cleanWhite(inStr);								// remove whitespaces

	WSposOld = 0;
	StrLen = inStr.length();
	int j = 0;
	while(j < 64){														// extracts the data into a string array
		WSpos = inStr.find(" ", WSposOld);
		if(WSpos > StrLen) WSpos = StrLen;

		fooVec.push_back(inStr.substr(WSposOld, WSpos - WSposOld));
		WSposOld = WSpos + 1;

		if(WSpos >= StrLen) break;
	}

	return fooVec;
}

/**
 * disVector description
 * @param inVec [line vector to be displayed]
 */

void disVector(vector<string> &inVec){
	int i = 0;
	string tempDis = "|";
	while(i < inVec.size()){
		tempDis = tempDis + inVec[i++] + "|";
	}
	cout << tempDis << endl;
}


/**
 * disVectorBlk description
 * @param inBlock [block vector to be displayed]
 */

void disVectorBlk(vector<vector<string> > &inBlock){
	string tempDis;

	cout << "--------------- Vector Block ---------------" << endl;

	for(int i = 0; i < inBlock.size(); i++){
		tempDis = "|";
		for(int j = 0; j < inBlock[i].size(); j++){
			tempDis = tempDis + inBlock[i][j] + "|";
		}
		cout << tempDis << endl;
	}

	cout << "------------- Vector Block END -------------" << endl;
}

/**
 * [progressBar - Creates a progress bar]
 * [========                         ] - 12.23 %
 * @param currentVal [The current value]
 * @param totalVal   [The total value]
 */

void progressBar(unsigned int currentVal, unsigned int totalVal){
	string bar = "\r  [";
	// float percF = (float)currentVal / (float)totalVal * 100;
	// unsigned int percI = percF;
	unsigned int percI = (float)currentVal / (float)totalVal * 100;

	for(unsigned int i = 0; i <= percI /2; i++) // 100/2 = 50 bars
		bar = bar + "=";
	for(unsigned int i = percI /2; i < 50; i++) // 100/2 = 50 bars
		bar = bar + " ";
	bar = bar + "] - ";

	cout << bar << percI << "%";
}

/**
 * Remove starting and trailing whitespaces, ensure only 1 space between parameter
 * @param  inStr [The input string that needs the leading and tailing whitespaces to be removed]
 * @return       [The cleaned string]
 */

string cleanWhite(string inStr){
	string tempStr = inStr;
	char tempChar;

	//Removes leading whitespaces
	tempChar = (char)tempStr.front();
	while(tempChar == ' ' || tempChar == '\t'){
		tempStr.erase(0, 1);
		tempChar = (char)tempStr.front();
	}

	//Remove multiple whitespaces
	int wsPos = tempStr.find("  ");
	while(wsPos != string::npos){
		// cout << "found double whitespace" << endl;
		tempStr.erase(wsPos, 1);
		wsPos = tempStr.find("  ");
	}

	//Removes trailing whitespaces
	tempChar = (char)tempStr.back();
	while(tempChar == ' ' || tempChar == '\t'){
		tempStr.pop_back();
		tempChar = (char)tempStr.back();
	}

	return tempStr;
}

/**
 * Remove starting and trailing whitespaces
 * @param  inStr [The input string that needs the leading and tailing whitespaces to be removed]
 * @return       [The cleaned string]
 */

string removeLTwhite(string inStr){
	string tempStr = inStr;
	char tempChar;

	//Removes leading whitespaces
	tempChar = (char)tempStr.front();
	while(tempChar == ' ' || tempChar == '\t'){
		tempStr.erase(0, 1);
		tempChar = (char)tempStr.front();
	}

	//Removes trailing whitespaces
	tempChar = (char)tempStr.back();
	while(tempChar == ' ' || tempChar == '\t'){
		tempStr.pop_back();
		tempChar = (char)tempStr.back();
	}

	return tempStr;
}

/**
 * [fileRenamer - Cleans up file name and able to prepend or append a string]
 * @param  inName [Input file name]
 * @param  preFix [The string to be added at the front]
 * @param  suffix [The string to be added to the end]
 * @return        [The modified string]
 */

string fileRenamer(string inName, string preFix, string suffix){
	string foo;

	inName.erase(0, inName.find_last_of("/")+1);
	foo = inName.insert(0, preFix);

	inName.erase(inName.find_last_of("."), inName.length());
	foo = inName.insert(inName.length(), suffix);
	return foo;
}